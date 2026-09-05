# Experience

A learning project: a small stack-based scripting language and its interpreter, written from scratch in C.

## Why

This exists to explore how interpreters work under the hood — tokenizing text into commands, building a control-flow tree, executing it against a shared value stack — without leaning on a parser generator, bytecode VM, or existing language runtime. Expect a terse, "golf-like" script language and a hand-rolled macro system faking OOP in C, not production-grade software. It's a place to learn interpreter design and low-level C (memory management, manual data structures) by building the whole stack yourself.

## A taste of the language

```
push 2;push 2;push add;push print;
```
prints `4`. Everything operates on one shared value stack; `get n`/`set n` read and write absolute slots in it, and `push if`/`push for`/`push end` mark control-flow blocks.

Scripts can also load and parameterize other script files:
```
use for {0} to {6} do {push print}.txt;
```
which loads [for {} to {} do {}.txt](for%20%7B%7D%20to%20%7B%7D%20do%20%7B%7D.txt) with `0`, `6`, and `push print` as its arguments, printing `012345`.

See [COMMANDS.md](COMMANDS.md) for the full command reference and a step-by-step traced example.

## Build & run

```
gcc -std=gnu11 -g main.c -o main.exe
./main.exe
```

`-std=gnu11` (or similar pre-C23 standard) is required — the codebase defines its own `bool`/`TRUE`/`FALSE`, which collides with the `bool`/`true`/`false` keywords newer compilers introduce by default under C23.

The whole interpreter is a single translation unit: `main.c` `#include`s every other `.c` file directly, so there's nothing to link separately.

## Architecture

Four pieces, in a straight pipeline:

**parser** (text → `xp_command`s) → **control** (owns the commands and the execution tree, exposes `execute()`) → **analyzer** (builds the tree, interprets each command, holds the value stack) → **common** (the shared array/tree data structures and the macro system everything else is built on).

For the deeper structural notes — the component diagram, the faux-OOP macro system, known quirks — see [CLAUDE.md](CLAUDE.md) (written for AI coding assistants working in this repo, but just as useful for a human getting oriented).

## Status

Exploratory and evolving. Expect minimal error handling, occasional breaking changes, and core mechanics (like how arguments get passed into commands) still being reworked as the language grows.
