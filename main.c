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
   // return 0;
   // "if {0} then {...}"
   // "push {0}; push if; {...}; push end;"
   // char cmd[] = "push FALSE;push if;push a;push print;push end;push fini;push print;";
   // char cmd[] = "push 2;push 2;push add;push print;";
   char cmd[] = "push 0;\
push 10;\
push TRUE;\
push for;\
get 0;\
push 1;\
push add;\
set 0;\
get 0;\
get 1;\
push equals;\
set 2;\
get 0;\
push print;\
push end;";
   xp_program *program = xp_program_create(cmd);
   bind(program)->execute();
   program->free(NULL);
   return 0;
}