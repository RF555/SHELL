#ifndef STSHELL_LIB_H
#define STSHELL_LIB_H

#include <stdio.h>
#include <signal.h>

enum ALIVE {
    TRUE = 1,
    FALSE = 0
};

/**
 * Signal handler function for SIGINT (Ctrl+C).
 */
void handle_sigint();

#endif //STSHELL_LIB_H
