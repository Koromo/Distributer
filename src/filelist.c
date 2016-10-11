#include "filelist.h"
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

File* insertFile(File* head, File* node)
{
    node->next = head;
    return node;
}

File* removeFrontFile(File** head)
{
    File* const removed = *head;
    *head = (*head)->next;
    removed->next = NULL;
    return removed;
}

static File* get(File* head, size_t i)
{
    if (i == 0)
    {
        return head;
    }
    return get(head->next, i - 1);
}

static File* marge(File* l, File* r)
{
    File* head;
    File* tail;
    File* less;
    int comp;

    head = NULL;
    tail = NULL;

    while (l && r)
    {
        comp = strncmp(l->number, r->number, 7);
        if (comp == 0)
        {
            comp = strcmp(l->date, r->date);
        }

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
        less->next = NULL;

        if (!head)
        {
            head = less;
        }
        else
        {
            tail->next = less;
        }
        tail = less;
    }

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

File* sortFiles(File* head, size_t length)
{
    size_t half;
    File* left;
    File* leftTail;
    File* right;

    if (length <= 1)
    {
        return head;
    }

    half = length / 2;
    left = head;
    leftTail = get(left, half - 1);
    right = leftTail->next;
    leftTail->next = NULL;
    
    left = sortFiles(left, half);
    right = sortFiles(right, length - half);

    return marge(left, right);
}

size_t numFiles(const File* head)
{
    if (!head)
    {
        return 0;
    }
    return numFiles(head->next) + 1;
}

void freeFiles(File* head)
{
    if (head)
    {
        freeFiles(head->next);
        free(head);
    }
}

static File* make(const char* number, const char* date)
{
    File* const node = (File*)malloc(sizeof(File));
    if (!node)
    {
        fprintf(stderr, "Memerror.\n");
        exit(EXIT_FAILURE);
    }
    node->number = number;
    node->date = date;
    node->next = NULL;
    return node;
}

static File* unique(File* head)
{
    File* removed;

    if (!head->next)
    {
        return head;
    }

    head->next = unique(head->next);

    if (strncmp(head->number, head->next->number, 7) == 0)
    {
        removed = head;
        head = head->next;
        free(removed);
    }

    return head;
}

static DIR* openDir(const char* path)
{
    DIR* const dir = opendir(path);
    if (!dir)
    {
        fprintf(stderr, "Failed to open directory (%s).\n", path);
        exit(EXIT_FAILURE);
    }
    return dir;
}

static DIR* openLocalDir(const char* inPath, const char* name)
{
    char* totalPath;
    const size_t inPathLength = strlen(inPath);
    const size_t totalPathLength = inPathLength + strlen(name) + 1;
    DIR* dir;

    totalPath = (char*)malloc(sizeof(char) * totalPathLength + 1);
    if (!totalPath)
    {
        fprintf(stderr, "Memerror.");
        exit(EXIT_FAILURE);
    }

    totalPath[0] = '\0';
    strcat(totalPath, inPath);
    strcat(totalPath, "/");
    strcat(totalPath, name);

    dir = openDir(totalPath);
    free(totalPath);

    return dir;
}

static File* makeFileListImpl(const char* date, DIR* dir)
{
    File* head;
    struct dirent* ent;

    head = NULL;
    for (ent = readdir(dir); ent; ent = readdir(dir))
    {
        if (ent->d_name[0] != '.')
        {
            head = insertFile(head, make(ent->d_name, date));
        }
    }

    return head;
}

static File* back(File* head)
{
    if (head->next == NULL)
    {
        return head;
    }
    return back(head->next);
}

File* makeFileList(const char* path)
{
    File* head;
    File* tail;
    File* list;
    DIR* inPathDir;
    DIR* dateDir;
    struct dirent* inPathEnt;

    inPathDir = openDir(path);

    head = NULL;
    for (inPathEnt = readdir(inPathDir); inPathEnt; inPathEnt = readdir(inPathDir))
    {
        if (inPathEnt->d_name[0] == '.')
        {
            continue;
        }

        dateDir = openLocalDir(path, inPathEnt->d_name);
        list = makeFileListImpl(inPathEnt->d_name, dateDir);
        if (!head)
        {
            head = list;
        }
        else
        {
            tail->next = list;
        }
        tail = back(list);
    }

    head = sortFiles(head, numFiles(head));
    head = unique(head);

    return head;
}
