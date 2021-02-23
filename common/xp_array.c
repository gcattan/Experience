#include <stdlib.h>
#include <stdio.h>
#include "xp_array.h"
#include "../common/xp_utils.h"

#define SIZE 10

void xp_array_free(xp_array **instance)
{
    free((*instance)->data);
    (*instance)->data = NULL;
    *instance = NULL;
}

xp_array *xp_array_create()
{
    xp_array *ret = NEW(xp_array);
    ret->data = NULL;
    ret->len = 0;
    binding(xp_array, free);
    binding(xp_array, pop);
    binding(xp_array, first);
    binding(xp_array, last);
    binding(xp_array, push);
    binding(xp_array, get);
    binding(xp_array, set);
    return ret;
}

void xp_array_push(xp_array *a_array, void *a_datum)
{
    if (a_array->free_space - 1 < 0)
    {
        void **new_data = malloc(sizeof(void *) * (a_array->len + SIZE));
        memcpy(new_data, a_array->data, a_array->len);
        free(a_array->data);
        a_array->data = new_data;
        a_array->free_space = SIZE;
    }
    a_array->data[a_array->len++] = a_datum;
    --a_array->free_space;
}

void *xp_array_pop(xp_array *a_array)
{
    void *ret = xp_array_get(a_array, a_array->len - 1);
    a_array->data[a_array->len - 1] = NULL;
    --a_array->len;
    return ret;
}

void *xp_array_last(xp_array *a_array)
{
    return xp_array_get(a_array, a_array->len - 1);
}

void *xp_array_first(xp_array *a_array)
{
    return xp_array_get(a_array, 0);
}

void *xp_array_get(xp_array *a_array, int a_pos)
{
    return a_array->len > a_pos ? a_array->data[a_pos] : NULL;
}

void *xp_array_set(xp_array *a_array, int a_pos, void *a_datum)
{
    if (a_array->len > a_pos)
    {
        a_array->data[a_pos] = a_datum;
    }
}