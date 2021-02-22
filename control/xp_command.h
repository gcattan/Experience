#pragma once

typedef struct xp_command xp_command;

struct xp_command
{
    char *name;
    char *value;
};

void xp_command_free(xp_command **);

xp_command *xp_command_create(char *a_name, char *a_value);
