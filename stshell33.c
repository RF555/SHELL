#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_PIPES 3

void execute_command(char *args[MAX_ARGS], int input_fd, int output_fd) {
    // Fork to execute command
    pid_t pid = fork();

    if (pid == 0) { // Child process
        // Set signal handler for SIGINT (Ctrl+C)
        signal(SIGINT, SIG_DFL);

        // Redirect input/output if necessary
        if (input_fd != STDIN_FILENO) {
            if (dup2(input_fd, STDIN_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            if (dup2(output_fd, STDOUT_FILENO) < 0) {
                perror("dup2");
                exit(1);
            }
            close(output_fd);
        }

        // Execute command
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else if (pid < 0) {
        perror("fork");
        exit(1);
    }

    // Wait for child process to complete
    int status;
    wait(&status);
}

void execute_pipeline(char *args[MAX_ARGS], int n_args) {
    char *pipe_args[MAX_PIPES][MAX_ARGS];
    int pipe_fds[MAX_PIPES][2];
    int n_pipes = 0, n_comm = 0;
    memset(pipe_args, 0, sizeof(pipe_args));
    for (int i = 0; i < n_args; i++) {
        if (strcmp(args[i], "|") == 0) {
            if (n_pipes >= MAX_PIPES) {
                fprintf(stderr, "Error: too many pipes\n");
                return;
            }
            n_pipes++;
            n_comm = 0;
        } else {
            pipe_args[n_pipes][n_comm % MAX_ARGS] = args[i];
            pipe_args[n_pipes][n_comm % MAX_ARGS + 1] = NULL;
            n_comm++;
        }
    }
    memset(pipe_fds, 0, sizeof(pipe_fds));
    // Create pipes
    for (int i = 0; i <= n_pipes; i++) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            return;
        }
        // Determine input/output fds
        int input_fd = (i == 0) ? STDIN_FILENO : pipe_fds[i - 1][1];
        //output_fd = (i == n_pipes) ? STDOUT_FILENO : pipe_fds[i][1];
        // Execute command
        execute_command(pipe_args[i], input_fd, pipe_fds[i][1]);
        // Close pipe fds if not needed
        if (i > 0) {
            close(pipe_fds[i - 1][0]);
            close(pipe_fds[i - 1][1]);
        }
    }
}

int main() {
    char input[1024];
    char *args[MAX_ARGS];
    int n_args;
    int output_fd = STDOUT_FILENO;
    int sigint_flag = 0;

    for (size_t i = 0; i < MAX_ARGS; i++) {
        args[i] = NULL;
    }

    signal(SIGINT, SIG_IGN);

    while (1) {
        // Print the shell prompt
        printf("stshell> ");

        // Read a line of input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            if (sigint_flag) {
                sigint_flag = 0;
                continue;
            }
            break;
        }

        // Check for exit command
        if (strcmp(input, "exit\n") == 0) {
            break;
        }

        // Tokenize the input into arguments
        n_args = 0;
        args[n_args] = strtok(input, " \t\n|");
        while (args[n_args] != NULL) {
            n_args++;
            args[n_args] = strtok(NULL, " \n");
        }
        int pipe = 0;
        // Check for pipeline
        for (int i = 0; i < n_args; i++) {

            if (strcmp(args[i], "|") == 0) {
                execute_pipeline(args, n_args);
                output_fd = STDOUT_FILENO;
                pipe = 1;
                break;
            }
        }
        // Check for output redirection
        for (int i = 0; i < n_args - 1; i++) {
            if (strcmp(args[i], ">") == 0) {
                output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd < 0) {
                    perror("open");
                    output_fd = STDOUT_FILENO;
                    break;
                }
                memmove(args + i, args + i + 2, (n_args - i - 1) * sizeof(char *));
                n_args -= 2;
                i--;
            } else if (strcmp(args[i], ">>") == 0) {
                output_fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (output_fd < 0) {
                    perror("open");
                    output_fd = STDOUT_FILENO;
                    break;
                }
                memmove(args + i, args + i + 2, (n_args - i - 1) * sizeof(char *));
                n_args -= 2;
                i--;
            }
        }

        // Check for input redirection
        for (int i = 0; i < n_args - 1; i++) {
            if (strcmp(args[i], "<") == 0) {
                int input_fd = open(args[i + 1], O_RDONLY);
                if (input_fd < 0) {
                    perror("open");
                    break;
                }
                memmove(args + i, args + i + 2, (n_args - i - 1) * sizeof(char *));
                n_args -= 2;
                i--;
                execute_command(args, input_fd, output_fd);
                close(input_fd);
                output_fd = STDOUT_FILENO;
                break;
            }
        }


        // Execute the command
        if (n_args > 0 && pipe == 0) {
            execute_command(args, STDIN_FILENO, output_fd);
            output_fd = STDOUT_FILENO;
        }
    }

    return 0;
}