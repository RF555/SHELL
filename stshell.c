#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>


enum ALIVE {
    TRUE = 1,
    FALSE = 0
};

/**
 * Signal handler function for SIGINT (Ctrl+C).
 */
void handle_sigint() {
    printf("\n"); // Print newline after Ctrl+C
    signal(SIGINT, handle_sigint); // Reinstall the signal handler
}


int MAX_INPUT_LEN = 1024;
enum ALIVE is_alive = TRUE;

int main() {
    int i;
    char *argv[10];
    char command[MAX_INPUT_LEN];
    char *token;
//    char *prev_command[MAX_INPUT_LEN];
    char *output_file;
    char *input_file;
    int output_file_descriptor = -1;
    int input_file_descriptor = -1;
    pid_t pid;

    int line = 0;

    signal(SIGINT, handle_sigint); // Register signal handler for Ctrl+C

    while (is_alive) {
        /* Start of shell */
        printf("%d hello: ", line++);
        fflush(stdout);

        /* Get input from user and store it in `command` */
        fgets(command, MAX_INPUT_LEN, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* Seperate the input by first SPACE */
        token = strtok(command, " ");

        /* parse command line */
        i = 0;
        while (token != NULL) {
            /* Redirection OUTPUT by `>` OR `>>` */
            if (strcmp(token, ">") == 0 || strcmp(token, ">>") == 0) {
                /* Get the output file path */
                output_file = strtok(NULL, " ");
                if (output_file == NULL) {
                    fprintf(stderr, "Error: missing output file after %s\n", token);
                    break;
                }
                /* Distinguish between `>` and `>>` */
                if (strcmp(token, ">") == 0) {
                    output_file_descriptor = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                } else {
                    output_file_descriptor = open(output_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
                }
                if (output_file_descriptor < 0) {
                    perror("open");
                    break;
                }
                /* Redirect the standard output (stdout) to the file descriptor */
                dup2(output_file_descriptor, STDOUT_FILENO);
                close(output_file_descriptor);
                break;

                /* Redirection INPUT by `<` */
            } else if (strcmp(token, "<") == 0) {
                input_file = strtok(NULL, " ");
                if (input_file == NULL) {
                    fprintf(stderr, "Error: missing input file after %s\n", token);
                    break;
                }
                input_file_descriptor = open(input_file, O_RDONLY);
                if (input_file_descriptor < 0) {
                    perror("open");
                    break;
                }
                dup2(input_file_descriptor, STDIN_FILENO);
                close(input_file_descriptor);
                break;



                /* Handle piping */


//            } else if (strcmp(token, "|") == 0) {
//                if (pipe(pipes[num_pipes % 2]) < 0) {
//                    perror("pipe");
//                    break;
//                }
//                if ((pid = fork()) == -1) {
//                    perror("fork");
//                    break;
//                } else if (pid == 0) { // Child process
//                    dup2(pipes[num_pipes % 2][1], STDOUT_FILENO);
//                    close(pipes[num_pipes % 2][0]);
//                    close(pipes[num_pipes % 2][1]);
//                    /* Execute the first part of the pipe */
//                    argv[i] = NULL;
//                    execvp(argv[0], argv);
//                    perror("execvp");
//                    exit(1);
//                } else { // Parent process
//                    num_pipes++;
//                    dup2(pipes[num_pipes % 2][0], STDIN_FILENO);
//                    close(pipes[num_pipes % 2][0]);
//                    close(pipes[num_pipes % 2][1]);
//                    i = 0;
//                    token = strtok(NULL, " ");
//                    while (token != NULL) {
//                        argv[i] = token;
//                        token = strtok(NULL, " ");
//                        i++;
//                    }
//                    argv[i] = NULL;
//                    if (i == 0) {
//                        fprintf(stderr, "Error: missing command after %s\n", token);
//                        break;
//                    }
//                    /* If there are no more pipes, execute the last part of the pipe */
//                    if (token == NULL) {
//                        execvp(argv[0], argv);
//                        perror("execvp");
//                        exit(1);
//                    }
//                }



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


        /* Run CMD tools using Fork */
        pid = fork();
        if (pid == -1) {
            perror("Fork ERROR - can't get process ID");
        } else if (pid == 0) {
//            printf("(fork) Executing command: %s\n", argv[0]);
            execvp(argv[0], argv);
//            perror("Error executing command");
            exit(1);
        } else {
            wait(NULL);
//            printf("Command %s executed\n", argv[0]);
        }

    }
    return 0;
}
