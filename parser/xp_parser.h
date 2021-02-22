#pragma once

typedef struct xp_command xp_command;
typedef struct xp_array xp_array;

xp_command *parse(char[]);
xp_array *parse_multiple(char[]);
