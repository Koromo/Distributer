#ifndef _FILELIST_H_
#define _FILELIST_H_

#include <stddef.h>

typedef struct File_t
{
    const char* number;
    const char* date;
    struct File_t* next;
} File;

File* insertFile(File* head, File* node);
File* removeFrontFile(File** head);
File* sortFiles(File* head, size_t length);
size_t numFiles(const File* head);
void freeFiles(File* head);
File* makeFileList(const char* path);

#endif
