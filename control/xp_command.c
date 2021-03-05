#include "xp_command.h"

void xp_command_free(xp_command **command, any args)
{
    free(*command);
    *command = NULL;
}

xp_command *xp_command_create(char *a_name, char *a_value)
{
    xp_command *ret = NEW(xp_command);
    ret->name = a_name;
    ret->value = a_value;
    binding(xp_command, free);
    return ret;
}
