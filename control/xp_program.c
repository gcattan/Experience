#include "xp_program.h"

#include <stdlib.h>

void xp_program_free(xp_program **program)
{
    int cmd_len = (*program)->commands->len;
    xp_command *cmd = NULL;
    for (int i = 0; i < cmd_len; ++i)
    {
        cmd = xp_program_get_command(*program, i);
        bind(cmd)->free();
    }
    xp_array *cmds = (*program)->commands;
    xp_array_free(&cmds);
    xp_node *root = (*program)->root;
    xp_node_free(&root, 1);
    free(*program);
    *program = NULL;
}

xp_command *xp_program_get_command(xp_program *program, int a_pos)
{
    return xp_array_get(program->commands, a_pos);
}

xp_program *xp_program_create(char *a_str)
{
    xp_program *ret = NEW(xp_program);
    ret->commands = parse_multiple(a_str);
    ret->root = xp_node_create(NULL, xp_node_execute_root, NULL);
    analyze(ret);
    return ret;
}

void xp_program_execute(xp_program *program)
{
    program->root->execute(program->root);
}
