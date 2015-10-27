#include "filelist.h"
#include "markerlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EPSILON 0.00001

// Output put each marker informations
void printMarkers(FILE* file, const MarkerNode* head)
{
    const MarkerNode* m;
    const FileNode* f;

    fprintf(file, "Name\tDate\t\tNumber\n");
    for (m = head; m; m = m->next)
    {
        fprintf(file, "\n");
        for (f = m->files; f; f = f->next)
        {
            fprintf(file, "-%s\t%s\t%.7s\n", m->name, f->date, f->name);
        }
    }
}

// Returns dynamic string
const char* dynamicCopy(const char* str)
{
    int length;
    char* copy;

    length = strlen(str);
    copy = (char*)malloc(sizeof(char) * length + 1);
    if (!copy)
    {
        printf("Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, str);
    
    return copy;
}

// Read markers from file
MarkerNode* readMarkers(const char* path)
{
    MarkerNode* head;
    char name[20];
    double ratio;
    FILE* file;

    file = fopen(path, "r");
    if (!file)
    {
        printf("Failed to open file (%s).\n", path);
        exit(EXIT_FAILURE);
    }

    head = NULL;
    while (fscanf(file, "%19s%lf", name, &ratio) != EOF)
    {
        fgetc(file); // Skip '\n'
        head = insertMarkerNode(head, makeMarkerNode(dynamicCopy(name), ratio));
    }

    fclose(file);

    return head;
}

// Distribute files
MarkerNode* distribute(FileNode* filesHead, MarkerNode* markersHead)
{
    double sumRatio;
    int numFiles;
    int numMarkers;
    int numMarkFilesParRatio;
    int numMarkFiles;
    MarkerNode* it;
    MarkerNode* tmp;
    MarkerNode* distributted;
    int i;
    int r;

    // Calculate count of mark files par ratio
    sumRatio = 0;
    for (it = markersHead; it; it = it->next)
    {
        sumRatio += it->ratio;
    }
    
    numFiles = fileListLength(filesHead);
    numMarkFilesParRatio = (int)(numFiles / sumRatio + EPSILON);

    // Distribute with easy
    for (it = markersHead; it; it = it->next)
    {
        numMarkFiles = (int)(numMarkFilesParRatio * it->ratio + EPSILON);
        for (int i = 0; i < numMarkFiles; ++i)
        {
            it->files = insertFileNode(it->files, removeFrontFileNode(&filesHead));
        }
    }

    // Distribute remain files
    distributted = NULL;
    for (numMarkers = markerListLength(markersHead); filesHead; --numMarkers)
    {
        r = rand() % numMarkers;
        tmp = removeMarkerNode(&markersHead, r);
        tmp->files = insertFileNode(tmp->files, removeFrontFileNode(&filesHead));
        distributted = insertMarkerNode(distributted, tmp);
    }

    while (markersHead)
    {
        distributted = insertMarkerNode(distributted, removeMarkerNode(&markersHead, 0));
    }

    return distributted;
}

// Entry point
int main(int argv, char** argc)
{
    FileNode* filesHead;
    MarkerNode* markersHead;
    MarkerNode* it;
    const char* markersPath;
    const char* homeworksPath;
    const char* outPath;
    FILE* outFile;

    // Check argments
    if (argv != 4)
    {
        printf("Usage: %s markers files out\n", argc[0]);
        printf("markers: Markers file path.\n");
        printf("files: Target files path.\n");
        printf("out: Output path.\n");
        exit(EXIT_FAILURE);
    }

    markersPath = argc[1];
    homeworksPath = argc[2];
    outPath = argc[3];

    // Initialize marker list
    markersHead = readMarkers(markersPath);

    // Initialize random
    srand(time(NULL));
    rand();
    rand();

    // Do distribution
    filesHead = makeFileList(homeworksPath);
    markersHead = distribute(filesHead, markersHead);
    markerListLength(markersHead);
    markersHead = sortMarkerList(markersHead, markerListLength(markersHead));
    for (it = markersHead; it; it = it->next)
    {
        it->files = sortFileList(it->files, fileListLength(it->files));
    }

    // Show result
    outFile = fopen(outPath, "w");
    if (!outFile)
    {
        printf("Failed to open file (%s)\n.", outPath);
        exit(EXIT_FAILURE);
    }

    printMarkers(outFile, markersHead);

    fclose(outFile);

    for (it = markersHead; it; it = it->next)
    {
        free((void*)it->name);
    }

    freeMarkerList(markersHead);
    freeFileList(filesHead);

    return 0;
}
