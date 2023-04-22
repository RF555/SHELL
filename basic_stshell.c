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

int main() {
    int i;
    char *argv[MAX_INPUT];
    char command[MAX_COMMAND_LEN];
    char *token;

    // Register the signal handler for SIGINT
    signal(SIGINT, signal_handler);

    while (1) {
        printf("hello: ");
        fgets(command, MAX_COMMAND_LEN, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* parse command line */
        i = 0;
        token = strtok(command, " ");
        while (token != NULL) {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL)
            continue;

        /* for commands not part of the shell command language */
        if (fork() == 0) {
            execvp(argv[0], argv);
            wait(NULL);
        }
    }
}
