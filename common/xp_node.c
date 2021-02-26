#include "xp_node.h"
#include "stdlib.h"

void xp_node_free(xp_node **a_node, any a_recursively)
{
    xp_array *array = (*a_node)->children;
    if (a_recursively && array)
    {
        int len = array->len;
        for (int i = 0; i < len; ++i)
        {
            xp_node *node = bind(array)->get(i);
            xp_node_free(&node, a_recursively);
        }
    }
    bind(array)->free(NULL);
    if ((*a_node)->parent)
    {
        array = (*a_node)->parent->children;
        bind(array)->set((*a_node)->index_in_parent, NULL);
    }
    free(*a_node);
    *a_node = NULL;
}

xp_node *xp_node_create(xp_node *a_parent, dlgt_execute execute, xp_command *a_cmd)
{
    xp_node *ret = NEW(xp_node);
    ret->execute = execute;
    ret->parent = a_parent;
    ret->children = xp_array_create();
    ret->command = a_cmd;
    if (a_parent)
    {
        xp_array *array = a_parent->children;
        bind(array)->push(ret);
        ret->index_in_parent = a_parent->children->len - 1;
    }
    else
    {
        ret->index_in_parent = -1;
    }
    binding(xp_node, push);
    binding(xp_node, get_child_at);
    binding(xp_node, children_num);
    binding(xp_node, free);
    return ret;
}

void xp_node_push(xp_node *a_parent, xp_node *a_child)
{
    xp_array *array = a_parent->children;
    bind(array)->push(a_child);
}

xp_node *xp_node_get_child_at(xp_node *a_parent, int a_pos)
{
    xp_array *array = a_parent->children;
    return bind(array)->get(a_pos);
}

int xp_node_children_num(xp_node *a_parent)
{
    return a_parent->children->len;
}