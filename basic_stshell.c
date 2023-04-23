#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>

#include "my_stshell.h"


int MAX_INPUT_LEN = 1024;
enum ALIVE is_alive = TRUE;

int main() {
    int i;
    char *argv[10];
    char command[1024];
    char *token;

    int line=0;

    while (is_alive) {
        /* Start of shell. */
        printf("%d hello: ",line);

        /* Get input from user and store it in `command`. */
        fgets(command, MAX_INPUT_LEN, stdin);
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
