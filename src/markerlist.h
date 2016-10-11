#ifndef MARKERLIST_H_
#define MARKERLIST_H_

#include "filelist.h"
#include <stddef.h>

typedef struct Marker_t
{
    const char* name;
    double ratio;
    File* files;
    struct Marker_t* next;
} Marker;

void freeMarkers(Marker* head);
Marker* makeMarker(const char* name, double ratio);
size_t numMarkers(const Marker* head);
Marker* insertMarker(Marker* head, Marker* node);
Marker* removeMarker(Marker** head, size_t i);
Marker* sortMarkers(Marker* head, size_t length);

#endif
