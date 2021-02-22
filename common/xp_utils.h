#pragma once
#include <string.h>

#define NEW(X) malloc(sizeof(X))

#define THIS(X) this##X
#define binded_struct(NAME)   \
    typedef struct NAME NAME; \
    NAME *THIS(NAME);         \
    NAME *bind(NAME *a_this)  \
    {                         \
        THIS(NAME) = a_this;  \
        return a_this;        \
    };                        \
    struct NAME

typedef enum bool
{
    FALSE,
    TRUE
} bool;

bool str_eq(char *a_str1, char *a_str2)
{
    return strcmp(a_str1, a_str2) == 0;
}
