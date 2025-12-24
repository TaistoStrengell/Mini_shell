Simple UNIX Shell

## overview
A C implementation of a simple command-line shell interface. This is a project exercise of the book Operating System Concepts, 10th edition — Silberschatz, Galvin & Gagne, featured in chapter three.

## Current Features
* Command Execution: Parses and executes standard UNIX commands using fork(), execvp(), and wait().
* History: Supports repeating the last command (!!)
* Interactive: Provides a visual "osh>" prompt loop.
* Backround execution using "&" command
* I/O execution with ">" and ">>" commands
* built-in commands such as support for cd and exit
* Support for a single pipe command ("|")

## Testing
A sample input file `test_commands.txt` is included for automated testing.
Run it using input redirection:

To compile the shell:
gcc main.c -o osh
