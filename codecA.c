#include "codecA.h"
#include <ctype.h>

void encode_codecA(char *message, int message_len, char *encoded) {
    int i = 0;
    while (message[i] != '\0' && i < message_len) {
//    while (i < message_len) {
        if (islower(message[i])) {
            encoded[i] = toupper(message[i]);
        } else if (isupper(message[i])) {
            encoded[i] = tolower(message[i]);
        } else {
            encoded[i] = message[i];
        }
        ++i;
    }
}

void decode_codecA(char *message, int message_len, char *encoded) {
    encode_codecA(message, message_len, encoded);
}
