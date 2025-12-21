#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80

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

void execute_command(char **args) {
    int background = 0;
    
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork epäonnistui");
        return;
    }

    if (pid == 0) {
        execvp(args[0], args);
        perror("Komentoa ei löytynyt");
        exit(1);
    } 
    else {
        if (background == 0) {
            wait(NULL); 
        } else {
            printf("[Process started in background: %d]\n", pid);
        }
    }
}

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

int main(void) {
    char input[MAX_LINE];
    char last_command[MAX_LINE] = "";

    while (1) {
        printf("osh>");
        fflush(stdout);

        if (!read_input(input)) break; 
        if (input[0] == '\0') continue;

        if (!handle_history(input, last_command)) {
            continue; 
        }

        char **args = parse_arguments(input);

        if (args[0] == NULL) {
            free(args);
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            free(args);
            break;
        } 
        
        execute_command(args);
        
        free(args);
    }
    return 0;
}




