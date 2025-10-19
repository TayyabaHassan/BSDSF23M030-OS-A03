#include "shell.h"
#include <unistd.h>   // for chdir()
#include <string.h>   // for strcmp()
#include <stdlib.h>   // for exit()
#include <stdio.h>    // for printf()
#include <readline/readline.h>
#include <readline/history.h>
char* history[HISTORY_SIZE];
int history_count = 0;

char* read_cmd(char* prompt, FILE* fp) {
    // readline handles prompt display and editing
    char* input = readline(prompt);

    if (input == NULL) { // Ctrl+D (EOF)
        return NULL;

    }

    // Add non-empty commands to Readline’s internal history
    if (*input) {
        add_history(input);
    }

    return input;
}
char** tokenize(char* cmdline) {
    // Edge case: empty command line
    if (cmdline == NULL || cmdline[0] == '\0' || cmdline[0] == '\n') {
        return NULL;
    }

    char** arglist = (char**)malloc(sizeof(char*) * (MAXARGS + 1));
    for (int i = 0; i < MAXARGS + 1; i++) {
        arglist[i] = (char*)malloc(sizeof(char) * ARGLEN);
        bzero(arglist[i], ARGLEN);
    }

    char* cp = cmdline;
    int argnum = 0;

    while (*cp != '\0' && argnum < MAXARGS) {
        // Skip whitespace
        while (*cp == ' ' || *cp == '\t')
            cp++;

        if (*cp == '\0')
            break;

        // Handle special single-character tokens
        if (*cp == '<' || *cp == '>' || *cp == '|') {
            arglist[argnum][0] = *cp;
            arglist[argnum][1] = '\0';
            argnum++;
            cp++;  // move past the special symbol
            continue;
        }

        // Handle normal words
        char* start = cp;
        int len = 0;
        while (*cp != '\0' && *cp != ' ' && *cp != '\t'
               && *cp != '<' && *cp != '>' && *cp != '|') {
            len++;
            cp++;
        }

        strncpy(arglist[argnum], start, len);
        arglist[argnum][len] = '\0';
        argnum++;
    }

    arglist[argnum] = NULL;

    if (argnum == 0) {
        for (int i = 0; i < MAXARGS + 1; i++)
            free(arglist[i]);
        free(arglist);
        return NULL;
    }

    return arglist;
}



#include <string.h>

void add_to_history(const char* cmd) {
    if (cmd == NULL || strlen(cmd) == 0) return;

    // Duplicate command string
    char* copy = strdup(cmd);

    // If history full, free oldest and shift everything up
    if (history_count == HISTORY_SIZE) {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history_count--;
    }

    history[history_count++] = copy;
}

void show_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d  %s\n", i + 1, history[i]);
    }
}

#include <unistd.h>   // for chdir()
#include <string.h>   // for strcmp()
#include <stdlib.h>   // for exit()
#include <stdio.h>    // for printf()

int handle_builtin(char **arglist) {
    if (arglist[0] == NULL) {
        return 1; // Empty command
    }

    // 1. exit
    if (strcmp(arglist[0], "exit") == 0) {
        printf("Exiting shell...\n");
        exit(0); // terminate the shell process
    }

    // 2. cd
    else if (strcmp(arglist[0], "cd") == 0) {
        if (arglist[1] == NULL) {
            fprintf(stderr, "cd: expected argument\n");
        } else {
            if (chdir(arglist[1]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    }

    // 3. help
    else if (strcmp(arglist[0], "help") == 0) {
        printf("FCIT Shell - Built-in commands:\n");
        printf("  cd <dir>   - Change directory\n");
        printf("  exit       - Exit the shell\n");
        printf("  help       - Display this help message\n");
        printf("  jobs       - Show background jobs (not implemented yet)\n");
        return 1;
    }

    // 4. jobs (placeholder)
    else if (strcmp(arglist[0], "jobs") == 0) {
        printf("Job control not yet implemented.\n");
        return 1;
    }
	else if (strcmp(arglist[0], "history") == 0) {
    show_history();
    return 1;
}


    return 0; // Not a built-in
}
