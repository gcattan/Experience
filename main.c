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
   xp_program *p = xp_program_create(cmd);
   xp_program_execute(p);
   xp_program_free(&p);
   return 0;
}