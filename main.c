#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h> // open-function
#include <stdbool.h>

#define MAX_LINE 80

typedef struct {
    char **argv;        // Osoitin argumenttilistaan (menee execvp:lle)
    char *output_file;  // Tiedostonimi (jos uudelleenohjaus) tai NULL
    bool append_mode;   // true = ">>", false = ">"
    bool background;    // true = "&"
} CommandInfo;

/* Reads a line of input from the user into the provided buffer.
   Returns 1 if successful, 0 if end-of-file or error occurs.
*/
int read_input(char *buffer) {
    if (fgets(buffer, MAX_LINE, stdin) == NULL) {
        return 0;
    }

    int length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }

    return 1;
}
/* Parses the input string into an array of arguments.
   Returns dynamically allocated array of argument strings.
*/
char** parse_arguments(char *string) {
    int max_args = MAX_LINE / 2;
    char **args = malloc(sizeof(char*) * (max_args + 1));
    if (!args) return NULL; 

    int i = 0;
    
    for (char *token = strtok(string, " "); 
         token != NULL && i < max_args; 
         token = strtok(NULL, " ")) {
             
        args[i] = token;
        i++;
    }
    args[i] = NULL;
    return args;
}

// Parses command details such as output redirection and background execution, stores the info in CommandInfo struct.
void parse_command_details(char **args, CommandInfo *cmd) {
    cmd->argv = args;
    cmd->output_file = NULL;
    cmd->append_mode = false;
    cmd->background = false;

    int i = 0;
    while (args[i] != NULL) {
        if (strcmp(args[i], "&") == 0) {
            if (args[i + 1] == NULL) {
                cmd->background = true;
                args[i] = NULL;
            } else {
                printf("Syntax error: '&' must be the last argument.\n");
                args[0] = NULL;
                return; 
            }
        }
        else if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
            if (strcmp(args[i], ">>") == 0) {
                cmd->append_mode = true;
            } else {
                cmd->append_mode = false;
            }

            if (args[i + 1] != NULL) {
                cmd->output_file = args[i + 1];
                args[i] = NULL;
                i++;
            } else {
                printf("Syntax error: Output file missing after redirection.\n");
                args[0] = NULL;
                return;
            }
        }
        i++;
    }
}


/* Executes external commands using fork and execvp. Has option for background execution and output redirection.
   Waits for the command to finish 
*/
void execute_command(char **args) {
    CommandInfo cmd;
    parse_command_details(args, &cmd);

    if (cmd.argv[0] == NULL) {
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        if (cmd.output_file != NULL) {
            int flags = O_WRONLY | O_CREAT | (cmd.append_mode ? O_APPEND : O_TRUNC);
            int fd = open(cmd.output_file, flags, 0644);
            
            if (fd < 0) {
                perror("Failed to open output file");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        execvp(cmd.argv[0], cmd.argv);
        perror("No such command");
        exit(1);
    } 
    else {
        if (!cmd.background) {
            wait(NULL); 
        } else {
            printf("[Process started in background: %d]\n", pid);
        }
    }
}

/* Checks if input is "!!", and if so replaces it with the last command.
   If there is no last command, it notifies the user.
   Returns 1 if input is valid, 0 otherwise.
*/
int handle_history(char *input, char *last_command) {
    if (strcmp(input, "!!") == 0) {
        if (last_command[0] == '\0') {
            printf("No commands in history.\n");
            return 0;
        }
        printf("%s\n", last_command);
        strcpy(input, last_command);
    } 
    else {
        strcpy(last_command, input);
    }
    
    return 1;
}
/* Executes built-in commands such as 'cd' and 'exit'.
   Returns 2 for exit, 1 for cd, and 0 for non-built-in commands.
*/
int execute_built_in(char** args){
    if (strcmp(args[0], "exit") == 0) {
        return 2;
        }
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            printf("include the directory name as a parameter)\n");
        } 
        else {
            if (chdir(args[1]) != 0) {
                perror("osh"); 
            }
        }
        return 1;
    }
    return 0;
}


int main(void) {
    char input[MAX_LINE];
    char last_command[MAX_LINE] = "";

    //mainloop, reads, parses and executes commands
    while (1) {
        printf("osh>");
        fflush(stdout);

        if (!read_input(input)) break; 
        if (input[0] == '\0') continue;

        //checks for "!!" command
        if (!handle_history(input, last_command)) {
            continue; 
        }

        char **args = parse_arguments(input);

        if (args[0] == NULL) {
            free(args);
            continue;
        }
        //checks if command is built-in or included in the linux operating system
        int built_in_status = execute_built_in(args);
        if (built_in_status == 2) {
            free(args);
            break;
        } else if (built_in_status == 0) {
            execute_command(args);
        }
        free(args);
    }
    return 0;
    
}




