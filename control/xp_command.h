#pragma once
#include "../common/xp_utils.h"

binded_struct(xp_command)
{
    char *name;
    char *value;
    void (*free)();
};

void xp_command_free(xp_command **);

xp_command *xp_command_create(char *a_name, char *a_value);

void xp_command_free_this()
{
    xp_command_free(&THIS(xp_command));
}