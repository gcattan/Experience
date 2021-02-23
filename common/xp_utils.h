#pragma once
#include <string.h>

#define NEW(X) malloc(sizeof(X))

#define THIS(X) this##X

#define bind(X) xp_##X##_bind(X)

#define __free void (*free)()

#define binding_struct(NAME)        \
    typedef struct NAME NAME;       \
    NAME *THIS(NAME);               \
    void NAME##_free(NAME **);      \
    void NAME##_free_this()         \
    {                               \
        NAME##_free(&THIS(NAME));   \
    }                               \
    NAME *NAME##_bind(NAME *a_this) \
    {                               \
        THIS(NAME) = a_this;        \
        return a_this;              \
    };                              \
    struct NAME

#define binding(NAME, METHOD) ret->METHOD = NAME##_##METHOD##_this;

#define binding_declare(OBJ, RET, NAME) \
    RET OBJ##_##NAME(OBJ *);            \
    RET OBJ##_##NAME##_this() { return OBJ##_##NAME(THIS(OBJ)); };

#define binding_declare_1(OBJ, RET, NAME, TYPE, VALUE) \
    RET OBJ##_##NAME(OBJ *, TYPE);                     \
    RET OBJ##_##NAME##_this(TYPE VALUE) { return OBJ##_##NAME(THIS(OBJ), VALUE); }

typedef enum bool
{
    FALSE,
    TRUE
} bool;

bool str_eq(char *a_str1, char *a_str2)
{
    return strcmp(a_str1, a_str2) == 0;
}
