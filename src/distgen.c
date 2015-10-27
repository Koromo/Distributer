#include "filelist.h"
#include "markerlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    const char* homeworksPath;
    const char* outputPath;
    FILE* file;

    // Check argments
    if (argv != 3)
    {
        printf("Usage: %s files out\n", argc[0]);
        printf("files: Target files path.\n");
        printf("out: Output path.\n");
        exit(EXIT_FAILURE);
    }

    homeworksPath = argc[1];
    outputPath = argc[2];

    // Initialize marker list
    /// TODO: I want to read markers from file
    markersHead = NULL;
    markersHead = insertMarkerNode(markersHead, makeMarkerNode("C", 0.5));
    markersHead = insertMarkerNode(markersHead, makeMarkerNode("B", 1));
    markersHead = insertMarkerNode(markersHead, makeMarkerNode("A", 1));

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
    file = fopen(outputPath, "w");
    if (!file)
    {
        printf("Failed to open file (%s)\n.", outputPath);
        exit(EXIT_FAILURE);
    }

    printMarkers(file, markersHead);

    fclose(file);
    freeMarkerList(markersHead);
    freeFileList(filesHead);

    return 0;
}
