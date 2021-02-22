#include <string.h>
#include "xp_parser.h"
#include "../control/xp_command.h"
#include <stdlib.h>
#include "../common/xp_array.h"

xp_array *parse_multiple(char a_commands[])
{
  xp_array *ret = xp_array_create();
  xp_array *text_cmd = xp_array_create();
  char *cmd = strtok(a_commands, ";");
  while (cmd)
  {
    xp_array_push(text_cmd, cmd);
    cmd = strtok(NULL, ";");
  }
  xp_command *c = NULL;
  for (int i = 0; i < text_cmd->len; ++i)
  {
    c = parse(xp_array_get(text_cmd, i));
    xp_array_push(ret, c);
  }
  return ret;
}

xp_command *parse(char a_command[])
{
  char *name = strtok(a_command, " ");
  char *value = strtok(NULL, ";");
  return xp_command_create(name, value);
}