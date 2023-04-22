#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>

int MAX_INPUT = 10;
int MAX_COMMAND_LEN = 1024;

// Signal handler function for Ctrl+c
void signal_handler(int sig) {
    printf("\n");
}

// Enum for redirect modes
typedef enum {
    /**
     * ">" mode.
     */
    WRITE_MODE,
    /**
     * ">>" mode.
     */
    APPEND_MODE
} RedirectMode;

int main() {
    int i;
    char *argv[MAX_INPUT];
    char command[MAX_COMMAND_LEN];
    char *token;
    int file_descriptor_out; // File descriptor for output file
    RedirectMode redirect_mode; // Mode of redirection


    // Register the signal handler for SIGINT
    signal(SIGINT, signal_handler);

    while (1) {
        printf("hello: ");
        fgets(command, MAX_COMMAND_LEN, stdin);
        command[strlen(command) - 1] = '\0'; // Replace \n with \0

        // Parse command line
        i = 0;
        token = strtok(command, " ");
        while (token != NULL) {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        // Is command empty
        if (argv[0] == NULL) {
            continue;
        }

        // Check for output redirection
        for (i = 0; argv[i] != NULL; i++) {
            if (strcmp(argv[i], ">") == 0) {
                // Output redirection with write mode
                redirect_mode = WRITE_MODE;
                file_descriptor_out = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC,
                                           S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
                if (file_descriptor_out == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                // Remove ">" and filename from argv
                memmove(&argv[i], &argv[i + 2], sizeof(argv[0]) * (i + 1));
                break;


            } else if (strcmp(argv[i], ">>") == 0) {
                // Output redirection WITH append mode
                redirect_mode = APPEND_MODE;
                file_descriptor_out = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND,
                                           S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
                if (file_descriptor_out == -1) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                // Remove ">>" and filename from argv
                memmove(&argv[i], &argv[i + 2], sizeof(argv[0]) * (i + 1));
                break;
            }
        }

        // For commands not part of the shell command language
        if (fork() == 0) {
            // Redirect output if necessary
            if (file_descriptor_out != STDOUT_FILENO) {
                if (dup2(file_descriptor_out, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(file_descriptor_out);
            }

            // Set signal handler for SIGINT
            struct sigaction sigint_action;
            sigint_action.sa_handler = SIG_DFL;
            sigemptyset(&sigint_action.sa_mask);
            sigint_action.sa_flags = 0;
            sigaction(SIGINT, &sigint_action, NULL);

            execvp(argv[0], argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // Wait for child process to finish
            wait(NULL);
            // Close output file if necessary
            if (file_descriptor_out != STDOUT_FILENO) {
                close(file_descriptor_out);
            }
        }
    }
}
