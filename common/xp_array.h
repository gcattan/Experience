#pragma once

#include "xp_utils.h"

binding_struct(xp_array)
{
    int len;
    int free_space;
    any *data;
    any (*pop)();
    any (*first)();
    any (*last)();
    void (*push)(any);
    any (*get)(int);
    any (*set)(int, any);
    __free;
};

xp_array *xp_array_create();

binding_declare(xp_array, any, pop);

binding_declare(xp_array, any, first);

binding_declare(xp_array, any, last);

binding_declare_1(xp_array, void, push, any, a_datum);

binding_declare_1(xp_array, any, get, int, a_pos);

binding_declare_2(xp_array, any, set, int, a_pos, any, a_datum);