#pragma once

#include "../parser/xp_parser.h"
#include "../common/xp_utils.h"
#include "../common/xp_array.h"
#include "../common/xp_node.h"
#include "../analyzer/xp_analyzer.h"
#include "xp_command.h"

binding_struct(xp_program)
{
    xp_array *commands;
    xp_node *root;
    void (*execute)();
    xp_command *(*get_command)(int);
    __free;
};

xp_program *xp_program_create(char *a_str);

binding_declare(xp_program, void, execute);

binding_declare_1(xp_program, xp_command *, get_command, int, i);
