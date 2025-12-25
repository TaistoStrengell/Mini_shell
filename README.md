# Simple UNIX Shell

## Overview
A C implementation of a simple command-line shell interface. This is a project exercise based on Chapter 3 of the book *Operating System Concepts, 10th Edition* by Silberschatz, Galvin & Gagne.

## Current Features
* **Command Execution:** Parses and executes standard UNIX commands using `fork()`, `execvp()`, and `waitpid()`.
* **History:** Supports repeating the last command using `!!`.
* **Interactive:** Provides a visual `osh>` prompt loop.
* **Background Execution:** Supports running commands in the background using `&`. Includes signal handling to automatically reap zombie processes.
* **Redirection:** Supports output redirection using `>` (truncate) and `>>` (append).
* **Built-in Commands:** Support for `cd` and `exit`.
* **Piping:** Support for a single pipe command (`|`).



## Compilation
To compile the shell, use the following command:
gcc -Wall -Wextra main.c -o osh



## Testing
A sample input file test_commands.txt is included for automated testing. You can run it using input redirection:
./osh < test_commands.txt

## Known Limitations
* **Argument Parsing:** Arguments regarding command logic must be placed before redirection operators. Flags placed after redirection are currently ignored.
* **Line Length:** The maximum command length is currently limited to 80 characters.
* **Multiple Pipes:** Only single piping is supported
* **Quote Handling:** The shell does not handle quoting logic. Quotes are passed literally to the command and do not group arguments containing spaces.
