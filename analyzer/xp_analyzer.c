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
    xp_array *array = getContext();
    char *condition = bind(array)->last();
    if (str_eq(condition, "TRUE"))
    {
        int len = bind(node)->children_num();
        for (int i = 0; i < len; ++i)
        {
            bind(node)->execute_child(i);
        }
    }
}

void xp_node_execute_for(xp_node *node)
{
    // TODO
    xp_array *array = getContext();
    char *condition = bind(array)->last();
    while (str_eq(condition, "TRUE"))
    {
        int len = bind(node)->children_num();
        for (int i = 0; i < len; ++i)
        {
            bind(node)->execute_child(i);
        }
    }
}

void xp_node_execute_root(xp_node *node)
{
    int len = node->children->len;
    for (int i = 0; i < len; ++i)
    {
        xp_array *array = node->children;
        xp_node *child = bind(array)->get(i);
        child->execute(child);
    }
}

void xp_node_execute_default(xp_node *node)
{
    xp_command *cmd = node->command;
    xp_array *array = getContext();
    if CASE_N ("push")
    {
        if CASE_V ("print")
        {
            printf("%s", bind(array)->last());
            bind(array)->push(cmd->value);
        }
        else if CASE_V ("add")
        {
            int a = atoi(bind(array)->last());
            int b = atoi(bind(array)->get(array->len - 2));
            char str[21];
            sprintf(str, "%d", (a + b));
            bind(array)->push(cmd->value);
            bind(array)->push(str);
        }
        else if CASE_V ("equals")
        {
            int a = atoi(bind(array)->last());
            int b = atoi(bind(array)->get(array->len - 2));
            char str[21];
            sprintf(str, "%s", a == b ? "TRUE" : "FALSE");
            bind(array)->push(cmd->value);
            bind(array)->push(str);
        }
        else
        {
            bind(array)->push(cmd->value);
        }
    }
    else if CASE_N ("get")
    {
        int value = atoi(cmd->value);
        any datum = bind(array)->get(value);
        bind(array)->push(datum);
    }
    else if CASE_N ("set")
    {
        int value = atoi(cmd->value);
        any datum = bind(array)->last();
        bind(array)->set(value, datum);
    }
}

void analyze(xp_program *program)
{
    xp_node *node = program->root;
    int cmd_num = program->commands->len;
    for (int i = 0; i < cmd_num; ++i)
    {
        xp_array *array = program->commands;
        xp_command *cmd = bind(array)->get(i);
        if CASE_V ("if")
        {
            node = xp_node_create(node, xp_node_execute_if, cmd);
        }
        else if CASE_V ("end")
        {
            node = node->parent;
        }
        else if CASE_V ("for")
        {
            node = xp_node_create(node, xp_node_execute_for, cmd);
        }
        else
        {
            xp_node_create(node, xp_node_execute_default, cmd);
        }
    }
}
