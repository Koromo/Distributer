#include "filelist.h"
#include "markerlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EPSILON 0.00001

void printMarkers(const Marker* head)
{
    const Marker* m;
    const File* f;

    printf("Name\tDate\t\tNumber\n");
    for (m = head; m; m = m->next)
    {
        printf("\n");
        for (f = m->files; f; f = f->next)
        {
            printf("-%s\t%s\t%.7s\n", m->name, f->date, f->number);
        }
    }
}

const char* strCopy(const char* str)
{
    int length;
    char* copy;

    length = strlen(str);
    copy = (char*)malloc(sizeof(char) * length + 1);
    if (!copy)
    {
        fprintf(stderr, "Memerror.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, str);
    
    return copy;
}

Marker* readMarkers(const char* path)
{
    Marker* head;
    char name[20];
    double ratio;
    FILE* file;

    file = fopen(path, "r");
    if (!file)
    {
        fprintf(stderr, "Failed to open file (%s).\n", path);
        exit(EXIT_FAILURE);
    }

    head = NULL;
    while (fscanf(file, "%19s%lf", name, &ratio) != EOF)
    {
        fgetc(file); // Skip '\n'
        head = insertMarker(head, makeMarker(strCopy(name), ratio));
    }

    fclose(file);

    return head;
}

Marker* distribute(File* hw, Marker* markers)
{
    double sumRatio;
    int numHWs;
    int numHWsParRatio;
    int numMarkHWs;
    Marker* it;
    Marker* tmp;
    Marker* result;
    int i;
    int r;

    sumRatio = 0;
    for (it = markers; it; it = it->next)
    {
        sumRatio += it->ratio;
    }
    
    numHWs = numFiles(hw);
    numHWsParRatio = (int)(numHWs / sumRatio + EPSILON);

    for (it = markers; it; it = it->next)
    {
        numMarkHWs = (int)(numHWsParRatio * it->ratio + EPSILON);
        for (int i = 0; i < numMarkHWs; ++i)
        {
            it->files = insertFile(it->files, removeFrontFile(&hw));
        }
    }

    result = NULL;
    for (i = numMarkers(markers); hw; --i)
    {
        r = rand() % i;
        tmp = removeMarker(&markers, r);
        tmp->files = insertFile(tmp->files, removeFrontFile(&hw));
        result = insertMarker(result, tmp);
    }

    while (markers)
    {
        result = insertMarker(result, removeMarker(&markers, 0));
    }

    return result;
}

int main(int argv, char** argc)
{
    File* hw;
    Marker* markers;
    Marker* it;
    const char* markersPath;
    const char* hwPath;
    FILE* outFile;

    if (argv != 3)
    {
        printf("Usage: %s markers homeworks\n", argc[0]);
        printf("markers: Markers file path.\n");
        printf("files: HW files path.\n");
        exit(EXIT_FAILURE);
    }

    markersPath = argc[1];
    hwPath = argc[2];

    markers = readMarkers(markersPath);

    srand(time(NULL));
    rand();
    rand();

    hw = makeFileList(hwPath);
    markers = distribute(hw, markers);
    markers = sortMarkers(markers, numMarkers(markers));
    for (it = markers; it; it = it->next)
    {
        it->files = sortFiles(it->files, numFiles(it->files));
    }

    printMarkers(markers);

    for (it = markers; it; it = it->next)
    {
        free((void*)it->name);
    }

    freeMarkers(markers);
    freeFiles(hw);

    return 0;
}
