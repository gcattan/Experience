#include "xp_node.h"
#include "stdlib.h"

void xp_node_free(xp_node **a_node, bool a_recursively)
{
    xp_array *children = (*a_node)->children;
    if (a_recursively && children)
    {
        int len = children->len;
        for (int i = 0; i < len; ++i)
        {
            xp_node *child = xp_array_get(children, i);
            xp_node_free(&child, a_recursively);
        }
    }
    xp_array_free(&children);
    if ((*a_node)->parent)
    {
        xp_array_set((*a_node)->parent->children, (*a_node)->index_in_parent, NULL);
    }
    free(*a_node);
    *a_node = NULL;
}

xp_node *xp_node_create(xp_node *a_parent, dlgt_execute execute, xp_command *a_cmd)
{
    xp_node *node = NEW(xp_node);
    node->execute = execute;
    node->parent = a_parent;
    node->children = xp_array_create();
    node->command = a_cmd;
    if (a_parent)
    {
        xp_array_push(a_parent->children, node);
        node->index_in_parent = a_parent->children->len - 1;
    }
    else
    {
        node->index_in_parent = -1;
    }
    return node;
}

void xp_node_push(xp_node *a_parent, xp_node *a_child)
{
    xp_array_push(a_parent->children, a_child);
}

xp_node *xp_node_get_child_at(xp_node *a_parent, int a_pos)
{
    return xp_array_get(a_parent->children, a_pos);
}

int xp_node_children_num(xp_node *a_parent)
{
    return a_parent->children->len;
}