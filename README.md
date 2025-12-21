Simple UNIX Shell

A C implementation of a simple command-line shell interface. This is a project exercise of the book Operating System Concepts, 10th edition — Silberschatz, Galvin & Gagne, featured in chapter three.

Current Features
* Command Execution: Parses and executes standard UNIX commands using fork(), execvp(), and wait().
* History: Supports repeating the last command (!!)
* Interactive: Provides a visual "osh>" prompt loop.

Roadmap (To-Do)
* Background Execution: Implementation for starting processes in the background
* I/O Redirection: Implementation of input redirection.
* Pipes: Inter-process communication between commands.
* Built-in Commands: Implementation of for directory navigation.


To compile the shell:
gcc main.c -o osh
