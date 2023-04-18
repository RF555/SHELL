#include "codecB.h"

void encode_codecB(char *message, int message_len, char *encoded) {
    int i = 0;
    while (message[i] != '\0' && i < message_len) {
        encoded[i] = (message[i] + 3) % 255;
        i++;
    }
}

void decode_codecB(char *message, int message_len, char *encoded) {
    int i = 0;
    while (message[i] != '\0' && i < message_len) {
        encoded[i] = (message[i] - 3) % 255;
        i++;
    }
}
