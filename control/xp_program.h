#pragma once

#include "../parser/xp_parser.h"
#include "../common/xp_utils.h"
#include "../common/xp_array.h"
#include "../common/xp_node.h"
#include "../analyzer/xp_analyzer.h"
#include "xp_command.h"

typedef struct xp_program
{
    xp_array *commands;
    xp_node *root;
} xp_program;

void xp_program_execute(xp_program *program);

void xp_program_free(xp_program **);

xp_program *xp_program_create(char *a_str);

xp_command *xp_program_get_command(xp_program *, int);