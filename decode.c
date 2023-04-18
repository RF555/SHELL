#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codecA.h"
#include "codecB.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: decode <codec> <message>\n");
        return 1;
    }

    char *codec_name = argv[1];
    char *message = argv[2];
    int message_len = strlen(message);

    // Check if message is surrounded by quotes
    if ((message[0] == '\"' && message[message_len - 1] == '\"') ||
        (message[0] == '\'' && message[message_len - 1] == '\'')) {
        message[message_len - 1] = '\0';
        message++;
        message_len -= 2;
    }

    if (strcmp(codec_name, "codecA") == 0) {
        char *decoded = (char *) malloc(message_len + 1);
        decode_codecA(message, message_len, decoded);
        printf("Decoded message: %s\n", decoded);
        free(decoded);
    } else if (strcmp(codec_name, "codecB") == 0) {
        char *decoded = (char *) malloc(message_len + 1);
        decode_codecB(message, message_len, decoded);
        printf("Decoded message: %s\n", decoded);
        free(decoded);
    } else {
        printf("Invalid codec: %s\n", codec_name);
        return 1;
    }

    return 0;
}
