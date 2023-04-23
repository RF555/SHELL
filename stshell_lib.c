#include "stshell_lib.h"

void handle_sigint() {
    printf("\n"); // Print newline after Ctrl+C
    signal(SIGINT, handle_sigint); // Reinstall the signal handler
}

