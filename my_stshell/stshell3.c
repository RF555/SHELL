#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX_ARGS 10
#define MAX_COMMAND_LENGTH 1024

/* Signal handler function for SIGINT (Ctrl+C) */
void handle_sigint() {
    printf("\n"); // Print newline after Ctrl+C
}

int main() {
    int i, in_fd, out_fd;
    char *argv[MAX_ARGS], *input_file, *output_file;
    char command[MAX_COMMAND_LENGTH], *token;
    int pipes[2][2]; // Array of pipe file descriptors
    int num_pipes; // Number of pipes to be created
    pid_t pid;

    signal(SIGINT, handle_sigint); // Register signal handler for Ctrl+C

    while (1) {
        printf("my_stshell: ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strlen(command) - 1] = '\0'; // Replace newline with null terminator

        /* Parse command line */
        i = 0;
        token = strtok(command, " ");
        while (token != NULL) {
            /* Handle input/output redirection */
            if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) {
                output_file = strtok(NULL, " ");
                if (output_file == NULL) {
                    fprintf(stderr, "Error: missing output file after %s\n", token);
                    break;
                }
                if (strcmp(token, ">") == 0) {
                    out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                } else {
                    out_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
                }
                if (out_fd < 0) {
                    perror("open");
                    break;
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
                break;
            } else if (strcmp(token, "<") == 0) {
                input_file = strtok(NULL, " ");
                if (input_file == NULL) {
                    fprintf(stderr, "Error: missing input file after %s\n", token);
                    break;
                }
                in_fd = open(input_file, O_RDONLY);
                if (in_fd < 0) {
                    perror("open");
                    break;
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
                break;
            }
            /* Handle piping */
            if (strcmp(token, "|") == 0) {
                if (pipe(pipes[num_pipes % 2]) < 0) {
                    perror("pipe");
                    break;
                }
                if ((pid = fork()) == -1) {
                    perror("fork");
                    break;
                } else if (pid == 0) { // Child process
                    dup2(pipes[num_pipes % 2][1], STDOUT_FILENO);
                    close(pipes[num_pipes % 2][0]);
                    close(pipes[num_pipes % 2][1]);
                    /* Execute the first part of the pipe */
                    argv[i] = NULL;
                    execvp(argv[0], argv);
                    perror("execvp");
                    exit(1);
                } else { // Parent process
                    num_pipes++;
                    dup2(pipes[num_pipes % 2][0], STDIN_FILENO);
                    close(pipes[num_pipes % 2][0]);
                    close(pipes[num_pipes % 2][1]);
                    i = 0;
                    token = strtok(NULL, " ");
                    while (token != NULL) {
                        argv[i] = token;
                        token = strtok(NULL, " ");
                        i++;
                    }
                    argv[i] = NULL;
                    if (i == 0) {
                        fprintf(stderr, "Error: missing command after %s\n", token);
                        break;
                    }
                    /* If there are no more pipes, execute the last part of the pipe */
                    if (token == NULL) {
                        execvp(argv[0], argv);
                        perror("execvp");
                        exit(1);
                    }
                }
            } else { // Regular command
                argv[i] = token;
                token = strtok(NULL, " ");
                i++;
            }
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL) {
            continue;
        }

        /* Exit command */
        if (strcmp(argv[0], "exit") == 0) {
            exit(0);
        }

        /* Fork and execute command */
        if ((pid = fork()) == -1) {
            perror("fork");
        } else if (pid == 0) { // Child process
            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        } else { // Parent process
            wait(NULL);
        }
    }
    return 0;
}