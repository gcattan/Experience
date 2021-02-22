#pragma once

#include "xp_array.h"
#include "../common/xp_array.h"
#include "../control/xp_command.h"

typedef struct xp_node xp_node;
typedef void (*dlgt_execute)(xp_node *);

struct xp_node
{
    dlgt_execute execute;
    xp_array *children;
    xp_node *parent;
    xp_command *command;
    int index_in_parent;
};

void xp_node_free(xp_node **, int a_recursively);

xp_node *xp_node_create(xp_node *parent, dlgt_execute execute, xp_command *a_cmd);

void xp_node_push(xp_node *a_parent, xp_node *a_child);

xp_node *xp_node_get_child_at(xp_node *a_parent, int a_pos);

int xp_node_children_num(xp_node *a_parent);