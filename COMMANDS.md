# Script Command Reference

This documents the `xp` script language interpreted by [analyzer/xp_analyzer.c](analyzer/xp_analyzer.c). There is no formal grammar or spec in the parser itself ([parser/xp_parser.c](parser/xp_parser.c) only tokenizes `name value;` pairs) — command *meaning* is defined entirely by the `if/else` chain in `xp_node_execute_default`, `xp_node_execute_if`, and `xp_node_execute_for`. This file is the missing spec, reverse-engineered from that code.

## Syntax

A script is a `;`-separated list of commands. Each command is `name value` (split on the *first* space):

```
push 2;push 2;push add;push print;
```

Everything is `name`/`value` — even control flow. `if`, `for`, and `end` are not command names; they are **values** of a `push` command (`push if`, `push for`, `push end`). `analyze()` (in xp_analyzer.c) scans each command's **value**, not its name, to decide whether to open/close a control-flow block — so `push if`/`push for` open a block and `push end` closes the nearest open one, before the command ever reaches runtime dispatch.

## The stack ("context")

There is one global stack, `g_context` (an `xp_array` of strings), shared across the whole program — including nested scripts loaded via `use`. Two things are easy to miss:

- `get n` / `set n` address **absolute** slot `n` in the array, not a position relative to the top. They're used as makeshift global variables, not stack-relative locals.
- `push`, `pop`, `last`, `first` operate on the actual top of the array (`array->len`).

## Command reference

| Command | Stack effect | Description |
|---|---|---|
| `push <val>` | pushes `<val>` | Pushes a literal string. `<val>` of `if`/`for`/`end` never reaches this — those are consumed by `analyze()` at parse time to build the block structure instead. |
| `push print` | peek, then push `"print"` | Prints the **current top** of stack (without popping it), then pushes the literal string `"print"` onto the stack. Note: nothing is popped, and the value pushed is the opcode name itself, not a result — this looks like a leftover artifact of how `add`/`equals` are implemented (see below), not the operator producing a computed value. |
| `push add` | pop-peek 2, push `"add"`, push sum | Reads the top two values (`last()` and the slot two below the new top, `get(len-2)`) as integers, pushes the literal `"add"`, then pushes their sum as a string. Note it doesn't pop the operands — they remain on the stack under the two new values. |
| `push equals` | pop-peek 2, push `"equals"`, push `"TRUE"`/`"FALSE"` | Same shape as `add`: compares the top two values as integers, pushes literal `"equals"`, then pushes `"TRUE"` or `"FALSE"`. This is the standard way to compute a loop/if condition. |
| `get <n>` | push `context[n]` | Duplicates absolute slot `n` onto the top of the stack. |
| `set <n>` | `context[n] = top` | Overwrites absolute slot `n` with the current top of stack. Does **not** pop the top. |
| `push if` | consumes nothing directly | Opens an `if` block (see Control flow below). |
| `push for` | consumes nothing directly | Opens a `for` (while-style) block. |
| `push end` | — | Closes the nearest open `if`/`for` block. |
| `use <template>` | none directly | Loads and recursively interprets another script file. `<template>` may contain `{...}` placeholders (see Control flow / `use`). |
| `geta <n>` | push `variables[n]` | Pushes the `n`-th `{...}` placeholder value captured by the enclosing `use`, if any. |
| `pop interpret` | pop 1, then whatever the parsed command does | Pops a string off the stack, parses it as one ad-hoc `name value` command, and executes it immediately — a minimal `eval`. |

## Control flow

`analyze()` builds an `xp_node` tree from the flat command list by watching for `if`/`for`/`end` as a command's **value**:

- `push if` — creates a child node executed by `xp_node_execute_if`. At runtime it peeks the current top of stack; if it equals the string `"TRUE"`, it executes its children once, in order.
- `push for` — creates a child node executed by `xp_node_execute_for`. At runtime it treats the top of stack as an **index into the context array** (`condition_index = atoi(last())`), then repeatedly executes its children while `context[condition_index] == "FALSE"`, re-reading that slot after each pass. In other words, the loop body is expected to update that slot (typically via `push equals` + `set`) to eventually flip it to `"TRUE"`.
- `push end` — pops back up to the parent block. It closes whatever block is currently open; there's no type-checking that an `if`'s `end` isn't accidentally closing a `for`.

## `use` and `geta`

`use <template>` lets one script load and run another as a nested program:

1. `get_variables()` scans `<template>` for `{...}` placeholders, extracting their contents as a list of variables and producing the literal filename with the placeholders left as bare `{}` (e.g. `for {0} to {6} do {push print}.txt` → variables `["0", "6", "push print"]`, filename `for {} to {} do {}.txt`).
2. The file at that path is read and interpreted as a brand-new `xp_program`, sharing the same global `g_context` stack as the caller.
3. Inside the nested script, `geta <n>` pushes the `n`-th captured variable onto the shared stack.
4. When the nested program finishes, the previous `g_filename`/`g_variables` are restored, so nested/recursive `use` calls compose correctly. `g_context` itself is never saved or restored — it's one continuous stack for the whole run.

This is how the repo's [for {} to {} do {}.txt](for%20%7B%7D%20to%20%7B%7D%20do%20%7B%7D.txt) template implements a parameterized loop: the *caller* supplies `start`, `stop`, and a body command via `{}` placeholders, and the *template* reads them back with `geta 0` / `geta 1` / `geta 2`.

## Illustrated example

From [main.c](main.c):

```
use for {0} to {6} do {push print}.txt;
```

This resolves to `g_filename = "for {} to {} do {}.txt"` and `g_variables = ["0", "6", "push print"]`, then runs that file:

```
geta 0;          // push start value "0"
geta 1;          // push stop value "6"
push FALSE;      // push initial loop condition
push 2;          // push 2 = the stack slot that will hold the loop condition
push for;        // open the loop, keyed to slot 2
  get 0;         // push current counter (slot 0)
  geta 2;        // push the body command, "push print"
  pop interpret; // pop it and execute "push print" right now -> prints the counter
  get 0;         // push counter again
  push 1;        // push 1
  push add;      // counter + 1 -> also overwrites slot 0 via the next set
  set 0;         // slot 0 = counter + 1
  get 0;         // push new counter
  get 1;         // push stop value (slot 1)
  push equals;   // is new counter == stop?
  set 2;         // slot 2 = "TRUE"/"FALSE" -> controls whether the loop continues
push end;
```

Walking the stack for the first loop iteration (slots shown as `[0]=... [1]=... ...`, `↩` marks a value pushed on top):

| Step | Command | Stack after | Notes |
|---|---|---|---|
| 1 | `geta 0` | `[0]="0"` | start value |
| 2 | `geta 1` | `[0]="0" [1]="6"` | stop value |
| 3 | `push FALSE` | `..[2]="FALSE"` | loop condition, initially not done |
| 4 | `push 2` | `..[3]="2"` | tells `for` where the condition lives |
| — | `push for` begins | condition = `context[2]` = `"FALSE"` → loop runs | |
| 5 | `get 0` | `..[4]="0"` | copy of counter |
| 6 | `geta 2` | `..[5]="push print"` | the body command as a string |
| 7 | `pop interpret` | `..` (slot 5 popped, "0" printed) `[5]="print"` | prints `0`, then pushes literal `"print"` |
| 8 | `get 0` | `..[6]="0"` | counter again |
| 9 | `push 1` | `..[7]="1"` | |
| 10 | `push add` | `..[8]="add" [9]="1"` | `0 + 1 = 1` |
| 11 | `set 0` | `[0]="1"` (overwritten) | counter is now 1 |
| 12 | `get 0` | `..[10]="1"` | |
| 13 | `get 1` | `..[11]="6"` | |
| 14 | `push equals` | `..[12]="equals" [13]="FALSE"` | `1 == 6`? no |
| 15 | `set 2` | `[2]="FALSE"` (unchanged) | loop continues |

The loop repeats, printing the counter *before* incrementing it each time, and stops as soon as the comparison in step 14 comes back `"TRUE"` (counter `== 6`). So the visible output is:

```
012345
```

— six digits, `0` through `5`; `6` itself is never printed because the loop condition flips to `"TRUE"` right after the counter becomes `6`, before another `pop interpret` runs.

### Known quirk

Notice the stack is never popped or shrunk during the loop (steps 5–15 above add ~9 new slots every iteration, and `push print`/`add`/`equals` each add an extra unused slot for their own opcode name). For a long-running loop this grows `g_context` unboundedly — worth keeping in mind before extending this interpreter to longer-running scripts.
