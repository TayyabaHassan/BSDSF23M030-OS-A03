#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_LEN 1024
#define MAXARGS 64
#define ARGLEN  64
#define PROMPT  "FCIT> "
#define HISTORY_SIZE 20

// Global variables
extern char* history[HISTORY_SIZE];
extern int history_count;

// Function prototypes
void add_to_history(const char* cmd);
void show_history();

char* read_cmd(char* prompt, FILE* fp);
char** tokenize(char* cmdline);
int execute(char* arglist[]);
int handle_builtin(char** arglist);

#endif // SHELL_H
