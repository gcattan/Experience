#include "../common/xp_array.h"
#include <stdio.h>

#define RUN(X)                      \
    printf("%s", #X);               \
    ++nbTest;                       \
    if (!X())                       \
    {                               \
        printf("%s", "\n--failed"); \
        ++nbFailed;                 \
    }                               \
    else                            \
    {                               \
        printf("%s", "\n--ok\n");   \
    }
#define ASSERT(CONDITION) \
    if (!CONDITION)       \
        return FALSE;

bool test_array_push_shouldNotBeEmpty()
{
    xp_array *array = xp_array_create();
    int *a = NEW(int);
    bind(array)->push(a);
    ASSERT(array->len == 1)
    free(a);
    bind(array)->free(NULL);
    return TRUE;
}

bool test_array_get_shouldReturnItem()
{
    xp_array *array = xp_array_create();
    int *a = NEW(int);
    *a = 2;
    bind(array)->push(a);
    bind(array)->get(0);
    ASSERT(*a == 2);
    free(a);
    bind(array)->free(NULL);
    return TRUE;
}

bool test_array_increase_shouldCopyItems()
{
    xp_array *array = xp_array_create();
    for (int i = 0; i < SIZE + 1; ++i)
    {
        int *a = NEW(int);
        *a = i;
        bind(array)->push(a);
    }
    for (int i = 0; i < SIZE + 1; ++i)
    {
        int *ptr = bind(array)->get(i);
        ASSERT(*ptr == i);
        free(ptr);
    }
    bind(array)->free(NULL);
    return TRUE;
}

void test_array()
{
    int nbTest = 0;
    int nbFailed = 0;
    RUN(test_array_push_shouldNotBeEmpty);
    RUN(test_array_get_shouldReturnItem);
    RUN(test_array_increase_shouldCopyItems);
}