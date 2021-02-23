#include "xp_command.h"
#include <stdlib.h>

void xp_command_free(xp_command **instance)
{
    free(*instance);
    *instance = NULL;
}

xp_command *xp_command_create(char *a_name, char *a_value)
{
    xp_command *ret = NEW(xp_command);
    ret->name = a_name;
    ret->value = a_value;
    binding(xp_command, free);
    return ret;
}
