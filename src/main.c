#include "shell.h"

int main() {
    char* cmdline;
    char** arglist;

    while ((cmdline = read_cmd(PROMPT, stdin)) != NULL) {
    	if ((arglist = tokenize(cmdline)) != NULL) {
    // Check if it's a built-in first
    		if (handle_builtin(arglist) == 0) {
   		     execute(arglist); // only run external if not built-in
    }

            // Free the memory allocated by tokenize()
            for (int i = 0; arglist[i] != NULL; i++) {
                free(arglist[i]);
            }
            free(arglist);
        }
        free(cmdline);
    }

    printf("\nShell exited.\n");
    return 0;
}
