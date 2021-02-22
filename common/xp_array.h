#pragma once

typedef struct xp_array
{
    int len;
    int free_space;
    void **data;
} xp_array;

void xp_array_free(xp_array **);

xp_array *xp_array_create();

void xp_array_push(xp_array *, void *a_datum);

void *xp_array_pop(xp_array *);

void *xp_array_first(xp_array *);

void *xp_array_last(xp_array *);

void *xp_array_get(xp_array *, int a_pos);

void *xp_array_set(xp_array *, int a_pos, void *a_datum);