#pragma once

typedef struct xp_command xp_command;

struct xp_command
{
    char *name;
    char *value;
    void (*free)();
};

void xp_command_free(xp_command **);

xp_command *xp_command_create(char *a_name, char *a_value);

xp_command *xp_command_this;

xp_command *bind(xp_command *a_this)
{
    xp_command_this = a_this;
    return a_this;
};

void xp_command_free_this()
{
    xp_command_free(&xp_command_this);
}