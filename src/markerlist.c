#include "markerlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeMarkers(Marker* head)
{
    if (head)
    {
        freeMarkers(head->next);
        free(head);
    }
}

Marker* makeMarker(const char* name, double ratio)
{
    Marker* node = (Marker*)malloc(sizeof(Marker));
    if (!node)
    {
        fprintf(stderr, "Memerror.\n");
        exit(EXIT_FAILURE);
    }

    node->name = name;
    node->ratio = ratio;
    node->files = NULL;
    node->next = NULL;

    return node;
}

size_t numMarkers(const Marker* head)
{
    if (!head)
    {
        return 0;
    }
    return numMarkers(head->next) + 1;
}

Marker* insertMarker(Marker* head, Marker* marker)
{
    marker->next = head;
    return marker;
}

static Marker* get(Marker* head, size_t i)
{
    if (i == 0)
    {
        return head;
    }
    return get(head->next, i - 1);
}

Marker* removeMarker(Marker** head, size_t i)
{
    Marker* tmp;
    Marker* removed;
    if (i == 0)
    {
        removed = (*head);
        *head = (*head)->next;
    }
    else
    {
        tmp = get(*head, i - 1);
        removed = tmp->next;
        tmp->next = removed->next;
    }
    removed->next = NULL;
    return removed;
}

static Marker* marge(Marker* l, Marker* r)
{
    Marker* head;
    Marker* tail;
    Marker* less;

    head = NULL;
    tail = NULL;

    while (l && r)
    {
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
    if (r)
    {
        tail->next = r;
    }

    return head;
}

Marker* sortMarkers(Marker* head, size_t length)
{
    size_t half;
    Marker* left;
    Marker* right;
    Marker* leftTail;

    if (length <= 1)
    {
        return head;
    }

    half = length / 2;
    left = head;
    leftTail = get(left, half - 1);
    right = leftTail->next;
    leftTail->next = NULL;
    
    left = sortMarkers(left, half);
    right = sortMarkers(right, length - half);

    return marge(left, right);
}
