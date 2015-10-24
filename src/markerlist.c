#include "markerlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeMarkerList(MarkerNode* head)
{
    if (head)
    {
        freeMarkerList(head->next);
        free(head);
    }
}

MarkerNode* makeMarkerNode(double rate, const char* name)
{
    MarkerNode* node = (MarkerNode*)malloc(sizeof(MarkerNode));
    if (!node)
    {
        printf("Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    node->rate = rate;
    node->name = name;
    node->files = NULL;
    node->next = NULL;

    return node;
}

size_t markerListLength(const MarkerNode* head)
{
    if (!head)
    {
        return 0;
    }
    return markerListLength(head->next) + 1;
}

MarkerNode* insertMarkerNode(MarkerNode* head, MarkerNode* node)
{
    node->next = head;
    return node;
}

// Returns i'th node
static MarkerNode* getNode(MarkerNode* head, size_t i)
{
    if (i == 0)
    {
        return head;
    }
    return getNode(head->next, i - 1);
}

MarkerNode* removeMarkerNode(MarkerNode** head, size_t i)
{
    MarkerNode* tmp;
    MarkerNode* removed;
    if (i == 0)
    {
        removed = (*head);
        *head = (*head)->next;
    }
    else
    {
        tmp = getNode(*head, i - 1);
        removed = tmp->next;
        tmp->next = removed->next;
    }
    removed->next = NULL;
    return removed;
}

// Marge marker list
static MarkerNode* marge(MarkerNode* l, MarkerNode* r)
{
    MarkerNode* head;
    MarkerNode* tail;
    MarkerNode* less;

    head = NULL;
    tail = NULL;

    while (l && r)
    {
        // Compare by name
        if (strcmp(l->name, r->name) < 0)
        {
            less = l;
            l = l->next;
        }
        else
        {
            less = r;
            r = r->next;
        }

        // Add less node to marged list
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
    if (r)
    {
        tail->next = r;
    }

    return head;
}

MarkerNode* sortMarkerList(MarkerNode* head, size_t length)
{
    size_t half;
    MarkerNode* left;
    MarkerNode* right;
    MarkerNode* leftTail;

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
    
    left = sortMarkerList(left, half);
    right = sortMarkerList(right, length - half);

    return marge(left, right);
}
