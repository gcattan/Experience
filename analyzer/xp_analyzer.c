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
    xp_array *array = getContext();
    int condition_index = atoi(bind(array)->last());
    char *condition = bind(array)->get(condition_index);
    while (str_eq(condition, "FALSE"))
    {
        int len = bind(node)->children_num();
        for (int i = 0; i < len; ++i)
        {
            bind(node)->execute_child(i);
        }
        condition = bind(array)->get(condition_index);
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

void print_context()
{
    printf("-------Context-------:");
    xp_array *array = getContext();
    for (int i = 0; i < array->len; ++i)
    {
        char *datum = bind(array)->get(i);
        printf("%s\n", datum);
    }
}

pack filename_and_variables = NULL;

void xp_node_execute_default(xp_node *node)
{
    xp_command *cmd = node->command;
    xp_array *array = getContext();
    // print_context();

    // printf("%s %s;\n", cmd->name, cmd->value);
    printf("%s", ""); // workaround: print not displayed
    if CASE_N ("push")
    {
        if CASE_V ("print")
        {
            char *str = bind(array)->last();
            printf("%s", str);
            bind(array)->push(cmd->value);
        }
        else if CASE_V ("add")
        {
            int a = atoi(bind(array)->last());
            int b = atoi(bind(array)->get(array->len - 2));
            char *str = malloc(sizeof(char) * 21);
            sprintf(str, "%d", (a + b));
            bind(array)->push(cmd->value);
            bind(array)->push(str);
        }
        else if CASE_V ("equals")
        {
            int a = atoi(bind(array)->last());
            int b = atoi(bind(array)->get(array->len - 2));
            char *str = malloc(sizeof(char) * 21);
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
    else if CASE_N ("use")
    {
        pack prev_config = filename_and_variables;
        filename_and_variables = get_variables(cmd->value);
        xp_array *variables = filename_and_variables[0];
        cstring v1 = xp_array_get(variables, 0);
        cstring filename = filename_and_variables[1];

        char *buffer = read_file(filename);
        xp_program *program = xp_program_create(buffer);
        bind(program)->execute();
        bind(program)->free(NULL);
        free(buffer);
        free(filename);
        xp_array_free(&variables, (any)TRUE);
        filename_and_variables = prev_config;
    }
    else if CASE_N ("geta")
    {
        if (filename_and_variables)
        {
            int i = atoi(cmd->value);
            xp_array *variables = filename_and_variables[0];
            cstring vari = xp_array_get(variables, i);
            bind(array)->push(vari);
        }
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
