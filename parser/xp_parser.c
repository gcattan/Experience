#include <string.h>
#include "xp_parser.h"
#include "../control/xp_command.h"
#include <stdlib.h>
#include "../common/xp_array.h"

xp_array *parse_multiple(char a_commands[])
{
  xp_array *ret = xp_array_create();
  xp_array *array = xp_array_create();
  char *cmd = strtok(a_commands, ";");
  while (cmd)
  {
    bind(array)->push(cmd);
    cmd = strtok(NULL, ";");
  }
  xp_command *c = NULL;
  for (int i = 0; i < array->len; ++i)
  {
    c = parse(bind(array)->get(i));
    xp_array_push(ret, c);
  }
  bind(array)->free(NULL);
  return ret;
}

xp_command *parse(char a_command[])
{
  char *name = strtok(a_command, " ");
  char *value = strtok(NULL, ";");
  return xp_command_create(name, value);
}