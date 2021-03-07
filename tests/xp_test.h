#include <stdio.h>

#define ASSERT(CONDITION) \
    if (!CONDITION)       \
        return FALSE;

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
