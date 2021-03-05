#pragma once
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define NEW(X) malloc(sizeof(X))

#define THIS(X) X##_this

#define bind(X) xp_##X##_bind(X)

#define __free void (*free)(any args)

#define binding_struct(NAME)             \
    typedef struct NAME NAME;            \
    NAME *THIS(NAME);                    \
    void NAME##_free(NAME **, any args); \
    void NAME##_free_this(any args)      \
    {                                    \
        NAME##_free(&THIS(NAME), args);  \
    }                                    \
    NAME *NAME##_bind(NAME *a_this)      \
    {                                    \
        THIS(NAME) = a_this;             \
        return a_this;                   \
    };                                   \
    struct NAME

#define binding(NAME, METHOD) ret->METHOD = NAME##_##METHOD##_this;

#define binding_declare(OBJ, RET, NAME) \
    RET OBJ##_##NAME(OBJ *);            \
    RET OBJ##_##NAME##_this() { return OBJ##_##NAME(THIS(OBJ)); };

#define binding_declare_1(OBJ, RET, NAME, TYPE, VALUE) \
    RET OBJ##_##NAME(OBJ *, TYPE);                     \
    RET OBJ##_##NAME##_this(TYPE VALUE) { return OBJ##_##NAME(THIS(OBJ), VALUE); }

#define binding_declare_2(OBJ, RET, NAME, TYPE1, VALUE1, TYPE2, VALUE2) \
    RET OBJ##_##NAME(OBJ *, TYPE1, TYPE2);                              \
    RET OBJ##_##NAME##_this(TYPE1 VALUE1, TYPE2 VALUE2) { return OBJ##_##NAME(THIS(OBJ), VALUE1, VALUE2); }

typedef void *any;
typedef enum bool
{
    FALSE,
    TRUE
} bool;

bool str_eq(char *a_str1, char *a_str2)
{
    return strcmp(a_str1, a_str2) == 0;
}

char *read_file(char filename[])
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(filename, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }

        fclose(f);
    }

    return buffer;
}

//https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char *trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if (str == NULL)
    {
        return NULL;
    }
    if (str[0] == '\0')
    {
        return str;
    }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while (isspace((unsigned char)*frontp))
    {
        ++frontp;
    }
    if (endp != frontp)
    {
        while (isspace((unsigned char)*(--endp)) && endp != frontp)
        {
        }
    }

    if (frontp != str && endp == frontp)
        *str = '\0';
    else if (str + len - 1 != endp)
        *(endp + 1) = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if (frontp != str)
    {
        while (*frontp)
        {
            *endp++ = *frontp++;
        }
        *endp = '\0';
    }

    return str;
}