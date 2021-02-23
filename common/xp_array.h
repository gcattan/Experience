#pragma once

#include "xp_utils.h"

binding_struct(xp_array)
{
    int len;
    int free_space;
    void **data;
    void *(*pop)();
    void *(*first)();
    void *(*last)();
    void (*push)(void *);
    void *(*get)(int);
    void *(*set)(int, void *);
    __free;
};

xp_array *xp_array_create();

binding_declare(xp_array, void *, pop);

binding_declare(xp_array, void *, first);

binding_declare(xp_array, void *, last);

binding_declare_1(xp_array, void, push, void *, a_datum);

binding_declare_1(xp_array, void *, get, int, a_pos);

binding_declare_2(xp_array, void *, set, int, a_pos, void *, a_datum);