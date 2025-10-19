#include "shell.h"

int main() {
    char* cmdline;
    char** arglist;

    while ((cmdline = read_cmd(PROMPT, stdin)) != NULL) {
	        // Handle !n re-execution before anything else
        if (cmdline[0] == '!' && strlen(cmdline) > 1) {
            int n = atoi(cmdline + 1);
            if (n > 0 && n <= history_count) {
                free(cmdline);
                cmdline = strdup(history[n - 1]);
                printf("%s\n", cmdline); // echo the command being re-executed
            } else {
                printf("Invalid history reference: %s\n", cmdline);
                free(cmdline);
                continue;
            }
        }
	        add_to_history(cmdline);


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
