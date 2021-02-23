#include <stdio.h>
#include "parser/xp_parser.c"
#include "analyzer/xp_analyzer.c"
#include "control/xp_program.c"
#include "control/xp_command.c"
#include "common/xp_array.c"
#include "common/xp_node.c"

int main()
{
   char cmd[] = "push a;push print;";
   xp_program *program = xp_program_create(cmd);
   bind(program)->execute();
   program->free();
   return 0;
}