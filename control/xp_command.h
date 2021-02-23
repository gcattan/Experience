#pragma once
#include "../common/xp_utils.h"

binding_struct(xp_command)
{
    char *name;
    char *value;
    __free;
};

xp_command *xp_command_create(char *a_name, char *a_value);
