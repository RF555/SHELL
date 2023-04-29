#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <signal.h>


#define MAX_INPUT_LEN 1024
#define MAX_ARGS 10
#define MAX_PIPE 10

#define PROMPT "\x1b[1;32m~stshell$ \x1b[0m"

pid_t main_pid;


/**
 * Signal handler function for SIGINT (Ctrl+C).
 */
void handle_sigint() {
    if (getpid() == main_pid) {
        printf("\n");
        signal(SIGINT, handle_sigint);
    } else {
        signal(SIGINT, SIG_DFL);
    }
}


int main() {
    char command[MAX_INPUT_LEN];
    char *argv[MAX_ARGS][MAX_INPUT_LEN];
    int i;
    int _args; // number of arguments
    int _pipes; // number of pipes
    int children_status; // children_status of children
    int pipes_fd[MAX_PIPE][2];
    pid_t p_id;
    main_pid = getpid();
    signal(SIGINT, handle_sigint);

    while (1) {
        /* Start of shell */
        printf(PROMPT);
        /* Get input from user and store it in `command` */
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';

        /* Exit command */
        if (strcmp(command, "exit") == 0) {
            break;
        }

        /* Seperate the commands input to pipes */
        char *commands[MAX_PIPE];
        _pipes = 0;
        commands[_pipes] = strtok(command, "|");
        while (commands[_pipes] != NULL) {
            commands[++_pipes] = strtok(NULL, "|");
        }

        /* Parse as command line */
        for (i = 0; i < _pipes; ++i) {

            /* Check for output redirection */
            char *redirect_cmd = NULL;
            char *redirect_over = strstr(commands[i], ">");
            char *redirect_app = strstr(commands[i], ">>");

            if (redirect_app != NULL) {
                redirect_cmd = redirect_app;
                *redirect_cmd = '\0';
                redirect_cmd += 2;
            } else if (redirect_over != NULL) {
                redirect_cmd = redirect_over;
                *redirect_cmd = '\0';
                redirect_cmd += 1;
            }

            /* Parse command to arguments */
            _args = 0;
            argv[i][_args] = strtok(commands[i], " ");
            while (argv[i][_args] != NULL) {
                argv[i][++_args] = strtok(NULL, " ");
            }
            argv[i][_args] = NULL;

            /* Create pipe */
            if (pipe(pipes_fd[i]) == -1) {
                perror("pipe");
                return 1;
            }

            /* Fork a process */
            p_id = fork();
            if (p_id == -1) {
                perror("fork");
                return 1;
            } else if (p_id == 0) {
                /* check if child process */
                if (i > 0) {
                    dup2(pipes_fd[i - 1][0], STDIN_FILENO);
                }
                if (i < _pipes - 1) {
                    dup2(pipes_fd[i][1], STDOUT_FILENO);
                }

                for (int j = 0; j < _pipes; ++j) {
                    if (j != i) {
                        close(pipes_fd[j][0]);
                        close(pipes_fd[j][1]);
                    }
                }

                /* If output redirection */
                if (redirect_cmd != NULL) {
                    int fd;
                    if (redirect_app != NULL) {
                        /* Open file for append */
                        fd = open(redirect_cmd, O_WRONLY | O_APPEND | O_CREAT, 0644);
                    } else {
                        /* Open file for overwrite */
                        fd = open(redirect_cmd, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                    }
                    if (fd == -1) {
                        perror("open");
                        exit(1);
                    }
                    /* Redirect stdout to file */
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                /* Execute command */
                if (execvp(argv[i][0], argv[i]) == -1) {
                    perror("execvp");
                    exit(1);
                }
            } else { // Parent process
                if (i > 0) {
                    close(pipes_fd[i - 1][0]);
                    close(pipes_fd[i - 1][1]);
                }
            }
        }

        /* Wait for children to finish */
        for (i = 0; i < _pipes; i++) {
            wait(&children_status);
        }
    }
    return 0;
}