#include "xp_program.h"

#include <stdlib.h>

void xp_program_free(xp_program **ref_program, any args)
{
    xp_program *program = *ref_program;
    int cmd_len = program->commands->len;
    xp_command *command = NULL;
    for (int i = 0; i < cmd_len; ++i)
    {
        command = bind(program)->get_command(i);
        bind(command)->free(NULL);
    }
    xp_array *array = program->commands;
    bind(array)->free(NULL);
    xp_node *node = program->root;
    bind(node)->free((any)TRUE);
    free(program);
    *ref_program = NULL;
}

xp_command *xp_program_get_command(xp_program *program, int a_pos)
{
    xp_array *array = program->commands;
    return bind(array)->get(a_pos);
}

xp_program *xp_program_create(char *a_str)
{
    xp_program *ret = NEW(xp_program);
    ret->commands = parse_multiple(a_str);
    ret->root = xp_node_create(NULL, xp_node_execute_root, NULL);
    binding(xp_program, free);
    binding(xp_program, execute);
    binding(xp_program, get_command);
    analyze(ret);
    return ret;
}

void xp_program_execute(xp_program *program)
{
    program->root->execute(program->root);
}
