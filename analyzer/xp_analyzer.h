#pragma once
#include "../common/xp_node.h"
#include "../control/xp_command.h"
#include "../common/xp_array.h"
#include "../common/xp_utils.h"
#include "../control/xp_program.h"

void print_context();

void analyze(xp_program *program);

void xp_node_execute_if(xp_node *node);

void xp_node_execute_for(xp_node *node);

void xp_node_execute_root(xp_node *node);

void xp_node_execute_default(xp_node *node);

xp_array *g_context = NULL;
cstring g_filename = NULL;
xp_array *g_variables = NULL;

xp_array *getContext();