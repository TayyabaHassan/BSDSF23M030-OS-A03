#include "shell.h"
#include <fcntl.h>
#include <unistd.h>

int execute(char* arglist[]) {
    int status;
    pid_t pid1, pid2;
    int pipe_index = -1;
    int fd[2];
    char* input_file = NULL;
    char* output_file = NULL;

    // Detect redirection and pipe
    for (int i = 0; arglist[i] != NULL; i++) {
        if (strcmp(arglist[i], "<") == 0) {
            input_file = arglist[i + 1];
            arglist[i] = NULL;
        } else if (strcmp(arglist[i], ">") == 0) {
            output_file = arglist[i + 1];
            arglist[i] = NULL;
        } else if (strcmp(arglist[i], "|") == 0) {
            pipe_index = i;
            arglist[i] = NULL;
            break;
        }
    }

    // Handle pipe
    if (pipe_index != -1) {
        char** left_cmd = arglist;
        char** right_cmd = &arglist[pipe_index + 1];

        char* right_output = NULL;
        char* right_input = NULL;

        // detect < or > on right side
        for (int i = 0; right_cmd[i] != NULL; i++) {
            if (strcmp(right_cmd[i], "<") == 0) {
                right_input = right_cmd[i + 1];
                right_cmd[i] = NULL;
            } else if (strcmp(right_cmd[i], ">") == 0) {
                right_output = right_cmd[i + 1];
                right_cmd[i] = NULL;
            }
        }

        if (pipe(fd) == -1) {
            perror("pipe failed");
            return -1;
        }

        pid1 = fork();
        if (pid1 == 0) { // Left child (writer)
            if (input_file != NULL) {
                int in_fd = open(input_file, O_RDONLY);
                if (in_fd < 0) {
                    perror("Input file open failed");
                    exit(1);
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execvp(left_cmd[0], left_cmd);
            perror("Command not found");
            exit(1);
        }

        pid2 = fork();
        if (pid2 == 0) { // Right child (reader)
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);

            if (right_output != NULL) {
                int out_fd = open(right_output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out_fd < 0) {
                    perror("Output file open failed");
                    exit(1);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }

            execvp(right_cmd[0], right_cmd);
            perror("Command not found");
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);
        waitpid(pid1, &status, 0);
        waitpid(pid2, &status, 0);
        return 0;
    }

    // Normal command (possibly with < or >)
    pid_t cpid = fork();
    if (cpid == -1) {
        perror("fork failed");
        return -1;
    }

    if (cpid == 0) { // Child
        int fd_in, fd_out;

        if (input_file != NULL) {
            fd_in = open(input_file, O_RDONLY);
            if (fd_in < 0) {
                perror("Input file open failed");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }

        if (output_file != NULL) {
            fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("Output file open failed");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        execvp(arglist[0], arglist);
        perror("Command not found");
        exit(1);
    } else {
        waitpid(cpid, &status, 0);
    }

    return 0;
}
