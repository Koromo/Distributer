#ifndef _FILELIST_H_
#define _FILELIST_H_

#include <stddef.h>

// File node
typedef struct FileNode_s
{
    const char* name;
    const char* date;
    struct FileNode_s* next;
} FileNode;

// Insert file node to front
FileNode* insertFileNode(FileNode* head, FileNode* node);

// Remove front file node
FileNode* removeFrontFileNode(FileNode** head);

// Sort file list by filename and date
FileNode* sortFileList(FileNode* head, size_t length);

// Returns length of file list
size_t fileListLength(const FileNode* head);

// Free memory of file list
void freeFileList(FileNode* head);

// Create file list from pass
FileNode* makeFileList(const char* path);

#endif
