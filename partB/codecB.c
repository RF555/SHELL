#include "codecB.h"

void encode_codecB(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = (str[i] + 3) % 255;
        i++;
    }
}

void decode_codecB(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = (str[i] - 3) % 255;
        i++;
    }
}
