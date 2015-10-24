#include "filelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

FileNode* insertFileNode(FileNode* head, FileNode* node)
{
    node->next = head;
    return node;
}

FileNode* removeFrontFileNode(FileNode** head)
{
    FileNode* removed;
    removed = *head;
    *head = (*head)->next;
    removed->next = NULL;
    return removed;
}

// Return i'th node
static FileNode* getNode(FileNode* head, size_t i)
{
    if (i == 0)
    {
        return head;
    }
    return getNode(head->next, i - 1);
}

// Marge file list
static FileNode* marge(FileNode* l, FileNode* r)
{
    FileNode* head;
    FileNode* tail;
    FileNode* less;
    int comp;

    head = NULL;
    tail = NULL;

    while (l && r)
    {
        // Compare head of l and r by number
        comp = strncmp(l->name, r->name, 7);

        // Recompare by date if the two has same number
        if (comp == 0)
        {
            comp = strcmp(l->date, r->date);
        }

        // Add less node to marged list
        if (comp < 0)
        {
            less = l;
            l = l->next;
        }
        else
        {
            less = r;
            r = r->next;
        }

        if (!head)
        {
            head = less;
        }
        else
        {
            tail->next = less;
        }
        tail = less;
        tail->next = NULL;
    }
    // Add remain nodes to marged list
    if (l)
    {
        tail->next = l;
    }
    else if (r)
    {
        tail->next = r;
    }

    return head;
}

FileNode* sortFileList(FileNode* head, size_t length)
{
    size_t half;
    FileNode* left;
    FileNode* leftTail;
    FileNode* right;

    if (length <= 1)
    {
        return head;
    }

    // Devide at half position
    half = length / 2;
    left = head;
    leftTail = getNode(left, half - 1);
    right = leftTail->next;
    leftTail->next = NULL;
    
    left = sortFileList(left, half);
    right = sortFileList(right, length - half);

    return marge(left, right);
}

size_t fileListLength(const FileNode* head)
{
    if (!head)
    {
        return 0;
    }
    return fileListLength(head->next) + 1;
}

void freeFileList(FileNode* head)
{
    if (head)
    {
        freeFileList(head->next);
        free(head);
    }
}

// Create node
static FileNode* makeNode(const char* filename, const char* date)
{
    FileNode* node = (FileNode*)malloc(sizeof(FileNode));
    if (!node)
    {
        printf("Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    node->name = filename;
    node->date = date;
    node->next = NULL;
    return node;
}

// Update to uniqued list by number
static FileNode* unique(FileNode* head)
{
    FileNode* removed;

    if (!head->next)
    {
        return head;
    }

    // Recursive call about tails
    head->next = unique(head->next);

    // Remove head if next of head has the same number
    if (strncmp(head->name, head->next->name, 7) == 0)
    {
        removed = head;
        head = head->next;
        free(removed);
    }

    return head;
}

// Open directoty
static DIR* openDirectory(const char* path)
{
    DIR* dir = opendir(path);
    if (!dir)
    {
        printf("Failed to open directory (%s).\n", path);
        exit(EXIT_FAILURE);
    }
    return dir;
}

// Open local directory
static DIR* openLocalDirectory(const char* parentPath, const char* basename)
{
    char* localPath;
    size_t parentPathLength;
    size_t localPathLength;
    DIR* localDir;

    parentPathLength = strlen(parentPath);
    localPathLength = parentPathLength + strlen(basename) + 1;

    localPath = (char*)malloc(sizeof(char) * localPathLength + 1);
    if (!localPath)
    {
        printf("Failed to allocate memory.");
        exit(EXIT_FAILURE);
    }

    localPath[0] = '\0';
    strcat(localPath, parentPath);
    strcat(localPath, "/");
    strcat(localPath, basename);

    localDir = openDirectory(localPath);
    free(localPath);

    return localDir;
}

// Create file list.
static FileNode* makeFileListImpl(const char* date, DIR* dir)
{
    FileNode* head;
    struct dirent* ent;

    head = NULL;
    for (ent = readdir(dir); ent; ent = readdir(dir))
    {
        if (ent->d_name[0] != '.')
        {
            head = insertFileNode(head, makeNode(ent->d_name, date));
        }
    }

    return head;
}

// Returns back node
static FileNode* getBackNode(FileNode* head)
{
    if (!head->next)
    {
        return head;
    }
    return getBackNode(head->next);
}

FileNode* makeFileList(const char* path)
{
    FileNode* head;
    FileNode* newListHead;
    FileNode* newListTail;
    DIR* pathDir;
    DIR* localDir;
    struct dirent* pathEnt;

    // Open path directory
    pathDir = openDirectory(path);

    head = NULL;
    for (pathEnt = readdir(pathDir); pathEnt; pathEnt = readdir(pathDir))
    {
        // Open local directory
        if (pathEnt->d_name[0] == '.')
        {
            continue;
        }
        localDir = openLocalDirectory(path, pathEnt->d_name);

        // Create file list.
        newListHead = makeFileListImpl(pathEnt->d_name, localDir);
        newListTail = getBackNode(newListHead);
        newListTail->next = head;
        head = newListHead;
    }

    // Make unique
    head = sortFileList(head, fileListLength(head));
    head = unique(head);

    return head;
}
