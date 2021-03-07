#include "../parser/xp_parser.h"
#include "../common/xp_array.h"
#include "xp_test.h"

bool test_get_variables_shouldReturnVariables()
{
    char test[] = "for {one} in {two}";
    xp_array *array = get_variables(test);
    ASSERT(array->len == 2);
    ASSERT(str_eq(bind(array)->get(0), "one"));
    ASSERT(str_eq(bind(array)->get(1), "two"));
}

void test_parser()
{
    int nbTest = 0;
    int nbFailed = 0;
    RUN(test_get_variables_shouldReturnVariables);
}