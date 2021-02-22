#include "xp_analyzer.h"
#include <stdio.h>

#define CASE_V(X) (str_eq(cmd->value, X))
#define CASE_N(X) (str_eq(cmd->name, X))

xp_array *getContext()
{
    if (g_context == NULL)
    {
        g_context = xp_array_create();
    }
    return g_context;
}

void xp_node_execute_if(xp_node *node)
{
    //TODO
}

void xp_node_execute_for(xp_node *node)
{
    //TODO
}

void xp_node_execute_root(xp_node *node)
{
    int len = node->children->len;
    for (int i = 0; i < len; ++i)
    {
        xp_node *child = xp_array_get(node->children, i);
        child->execute(child);
    }
}

void xp_node_execute_default(xp_node *node)
{
    xp_command *cmd = node->command;
    xp_array *context = getContext();
    if CASE_N ("push")
    {
        if CASE_V ("print")
        {
            printf("%s", xp_array_last(context));
        }
        xp_array_push(context, cmd->value);
    }
}

void analyze(xp_program *program)
{
    xp_node *current = program->root;
    int cmd_num = program->commands->len;
    for (int i = 0; i < cmd_num; ++i)
    {
        xp_command *cmd = xp_array_get(program->commands, i);
        if CASE_V ("if")
        {
            current = xp_node_create(current, xp_node_execute_if, cmd);
        }
        else if CASE_V ("end")
        {
            current = current->parent;
        }
        else if CASE_V ("for")
        {
            current = xp_node_create(current, xp_node_execute_for, cmd);
        }
        else
        {
            xp_node_create(current, xp_node_execute_default, cmd);
        }
    }
}
