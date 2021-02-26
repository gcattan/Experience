#pragma once

#include "xp_array.h"
#include "../common/xp_array.h"
#include "../control/xp_command.h"
#include "../common/xp_utils.h"

typedef struct xp_node xp_node;
typedef void (*dlgt_execute)(xp_node *);

binding_struct(xp_node)
{
    dlgt_execute execute;
    xp_array *children;
    xp_node *parent;
    xp_command *command;
    int index_in_parent;
    void (*push)(xp_node *);
    xp_node *(*get_child_at)(int);
    int (*children_num)();
    void (*execute_child)(int);
    __free;
};

xp_node *xp_node_create(xp_node *parent, dlgt_execute execute, xp_command *a_cmd);

binding_declare_1(xp_node, void, push, xp_node *, a_child);
binding_declare_1(xp_node, xp_node *, get_child_at, int, a_pos);
binding_declare_1(xp_node, void, execute_child, int, a_pos);

binding_declare(xp_node, int, children_num);
