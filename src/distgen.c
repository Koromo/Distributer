#include "filelist.h"
#include "markerlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EPSILON 0.00001

// Output put each marker informations
void printMarkers(const MarkerNode* head)
{
    const MarkerNode* m;
    const FileNode* f;

    printf("Name\tDate\t\tNumber\n");
    for (m = head; m; m = m->next)
    {
        printf("\n");
        for (f = m->files; f; f = f->next)
        {
            printf("-%s\t%s\t%.7s\n", m->name, f->date, f->name);
        }
    }
}

// Distribute files
MarkerNode* distribute(FileNode* filesHead, MarkerNode* markersHead)
{
    float sumRate;
    int numFiles;
    int numMarkers;
    int numMarkFilesParRate;
    int numMarkFiles;
    MarkerNode* it;
    MarkerNode* tmp;
    MarkerNode* distributted;
    int i;
    int r;

    // Calculate count of mark files par rate
    sumRate = 0;
    for (it = markersHead; it; it = it->next)
    {
        sumRate +=it->rate;
    }
    
    numFiles = fileListLength(filesHead);
    numMarkFilesParRate = (int)(numFiles / sumRate + EPSILON);

    // Distribute with easy
    for (it = markersHead; it; it = it->next)
    {
        numMarkFiles = (int)(numMarkFilesParRate * it->rate + EPSILON);
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
    const char* path;
    FILE* file;

    // Check argments
    if (argv != 2)
    {
        printf("Usage: %s files markers\n", argc[0]);
        printf("files: Target files path.\n");
        exit(EXIT_FAILURE);
    }

    path = argc[1];

    // Initialize marker list
    /// TODO: I want to read markers from file
    markersHead = NULL;
    markersHead = insertMarkerNode(markersHead, makeMarkerNode(0.5, "C"));
    markersHead = insertMarkerNode(markersHead, makeMarkerNode(1, "B"));
    markersHead = insertMarkerNode(markersHead, makeMarkerNode(1, "A"));

    // Initialize random
    srand(time(NULL));
    rand();
    rand();

    // Do distribution
    filesHead = makeFileList(path);
    markersHead = distribute(filesHead, markersHead);
    markerListLength(markersHead);
    markersHead = sortMarkerList(markersHead, markerListLength(markersHead));
    for (it = markersHead; it; it = it->next)
    {
        it->files = sortFileList(it->files, fileListLength(it->files));
    }

    // Show result
    printf("SUCCEEDED\n\n");
    printMarkers(markersHead);

    freeMarkerList(markersHead);
    freeFileList(filesHead);

    return 0;
}
