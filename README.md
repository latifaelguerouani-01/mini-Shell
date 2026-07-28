# mini-Shell

## Description

A mini Unix shell developed in C for Linux that executes system commands using process management system calls. The project supports built-in commands, signal handling, and pipe-based inter-process communication.

## Features

- Execute Linux commands
- Process creation using `fork()`
- Program execution using `execvp()`
- Process synchronization using `wait()`
- Signal handling (`SIGINT`, `SIGTSTP`)
- Inter-process communication using pipes (`|`)
- Built-in commands: `cd` and `exit`

## Compilation

```bash
make
```

Or compile manually:

```bash
gcc main.c -o mini-Shell
```

## Run

```bash
./mini-Shell
```

## Example

```bash
mini-Shell >>> ls
main.c  README.md

mini-Shell >>> ls | grep .c
main.c

mini-shell >>> cd ..

mini-shell >>> exit
Goodbye!
```

## Technologies

- C Programming Language
- Linux System Calls
- GCC Compiler
