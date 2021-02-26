#include <stdio.h>
#include "parser/xp_parser.c"
#include "analyzer/xp_analyzer.c"
#include "control/xp_program.c"
#include "control/xp_command.c"
#include "common/xp_array.c"
#include "common/xp_node.c"

#include "tests/xp_array_test.c"

int main()
{
   // test_array();
   char cmd[] = "push FALSE;push if;push a;push print;push end;push fini;push print;";
   xp_program *program = xp_program_create(cmd);
   bind(program)->execute();
   program->free(NULL);
   return 0;
}