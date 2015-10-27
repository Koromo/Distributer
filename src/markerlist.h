#ifndef MARKERLIST_H_
#define MARKERLIST_H_

#include "filelist.h"
#include <stddef.h>

// Marker node
typedef struct MarkerNode_s
{
    const char* name;
    double ratio;
    FileNode* files;
    struct MarkerNode_s* next;
} MarkerNode;

// Free memory of marker list
void freeMarkerList(MarkerNode* head);

// Create marker node
MarkerNode* makeMarkerNode(const char* name, double ratio);

// Returns length of marker list
size_t markerListLength(const MarkerNode* head);

// Insert marker node to front of list
MarkerNode* insertMarkerNode(MarkerNode* head, MarkerNode* node);

// Remove i'th marker node
MarkerNode* removeMarkerNode(MarkerNode** head, size_t i);

// Sort marker list by name
MarkerNode* sortMarkerList(MarkerNode* head, size_t length);

#endif
