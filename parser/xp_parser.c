#include <string.h>
#include "xp_parser.h"
#include "../control/xp_command.h"
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
    if (c)
      xp_array_push(ret, c);
  }
  bind(array)->free(NULL);
  return ret;
}

xp_command *parse(char a_command[])
{
  char *clean = trim(a_command);
  if (isprint((unsigned char)*clean))
  {
    char *name = strtok(clean, " ");
    char *value = strtok(NULL, ";");
    return xp_command_create(name, value);
  }
  return NULL;
}

// for {0} to {1}
xp_array *get_variables(char *string)
{
  xp_array *array = xp_array_create();
  char *ptr = string;
  int i = -1;
  int i_start = -1;
  int i_stop = -1;
  while (*ptr != '\0')
  {
    ++i;
    char c = (*ptr++);
    if (c == '{')
    {
      i_start = i + 1;
    }
    else if (c == '}')
    {
      i_stop = i - 1;
      int len = i_stop - i_start + 2;
      char *var = malloc(sizeof(char) * (len));
      strcpy(var, &(string[i_start]));
      var[len - 1] = '\0';
      bind(array)->push(var);
    }
  }
  return array;
}