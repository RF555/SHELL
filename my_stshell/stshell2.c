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
#define MAX_CMD_LEN 1024
#define MAX_PIPES 2

int num_pipes;
int pipe_fds[MAX_PIPES][2];
pid_t pid;

void handle_sigint() {
    if (pid > 0) {
        kill(pid, SIGINT); // send SIGINT to child process
    }
}

void setup_pipes(int cmd_idx) {
    if (cmd_idx < num_pipes) {
        pipe(pipe_fds[cmd_idx]); // create pipe
    }
}

void close_pipes(int cmd_idx) {
    if (cmd_idx > 0) {
        close(pipe_fds[cmd_idx - 1][1]); // close write end of previous pipe
        close(pipe_fds[cmd_idx - 1][0]); // close read end of previous pipe
    }
    if (cmd_idx < num_pipes) {
        close(pipe_fds[cmd_idx][0]); // close read end of current pipe
        close(pipe_fds[cmd_idx][1]); // close write end of current pipe
    }
}

void exec_cmd(char **argv, int in_fd, int out_fd) {
    if (in_fd != STDIN_FILENO) {
        dup2(in_fd, STDIN_FILENO); // redirect stdin
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO) {
        dup2(out_fd, STDOUT_FILENO); // redirect stdout
        close(out_fd);
    }
    execvp(argv[0], argv); // execute command
    perror("execvp"); // execvp returns only on error
    exit(EXIT_FAILURE);
}

void execute_cmds(char **cmds) {
    int in_fd = STDIN_FILENO;
    int i;
    for (i = 0; i < num_pipes + 1; i++) {
        int out_fd = STDOUT_FILENO;
        if (i < num_pipes) {
            out_fd = pipe_fds[i][1]; // set write end of pipe as stdout
        }
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // child process
            exec_cmd(cmds[i], in_fd, out_fd);
        } else { // parent process
            close_pipes(i);
            in_fd = pipe_fds[i][0]; // set read end of pipe as stdin
        }
    }
    pid_t wpid;
    int status;
    while ((wpid = wait(&status)) > 0); // wait for all child processes to finish
}

void parse_cmd(char *cmd) {
    char *cmds[MAX_PIPES + 1][MAX_ARGS + 1]; // array of commands and arguments
    num_pipes = 0;
    int cmd_idx = 0;
    int arg_idx = 0;
    char *token = strtok(cmd, " ");
    while (token != NULL) {
        if (strcmp(token, "|") == 0) { // found pipe
            cmds[cmd_idx][arg_idx] = NULL;
            num_pipes++;
            cmd_idx++;
            arg_idx = 0;
            setup_pipes(cmd_idx);
        } else {
            cmds[cmd_idx][arg_idx] = token;
            arg_idx++;
        }
        token = strtok(NULL, " ");
    }
    cmds[cmd_idx][arg_idx] = NULL; // add null terminator to last command
    execute_cmds(cmds);
}

int main() {
    signal(SIGINT, handle_sigint); // register signal handler for SIGINT (Ctrl+c)

    char cmd[MAX_CMD_LEN];
    while (1) {
        printf("my_stshell> ");
        fgets(cmd, MAX_CMD_LEN, stdin);
        cmd[strlen(cmd) - 1] = '\0'; // replace \n with \0

        if (strcmp(cmd, "exit") == 0) { // exit command
            break;
        }

        if (strlen(cmd) == 0) { // empty command
            continue;
        }

        parse_cmd(cmd); // parse and execute command
    }

    return 0;
}