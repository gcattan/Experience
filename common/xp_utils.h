#pragma once
#include <string.h>

#define NEW(X) malloc(sizeof(X))

int str_eq(char *a_str1, char *a_str2)
{
    return strcmp(a_str1, a_str2) == 0;
}
