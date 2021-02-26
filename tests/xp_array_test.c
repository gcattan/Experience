#include "../common/xp_array.h"
#include <stdio.h>

#define RUN(X) \
    ++nbTest;  \
    if (!X())  \
        ++nbFailed;
#define ASSERT(CONDITION) \
    if (!CONDITION)       \
        return FALSE;

bool test_array_push_shouldNotBeEmpty()
{
    xp_array *array = xp_array_create();
    int *a = NEW(int);
    array->push(a);
    ASSERT(array->len == 1)
    return TRUE;
}

void test_array()
{
    int nbTest = 0;
    int nbFailed = 0;
    RUN(test_array_push_shouldNotBeEmpty);
}