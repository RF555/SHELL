#include "codecA.h"
#include <ctype.h>

void encode_codecA(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (islower(str[i])) {
            str[i] = toupper(str[i]);
        } else if (isupper(str[i])) {
            str[i] = tolower(str[i]);
        }
        i++;
    }
}

void decode_codecA(char *str) {
    encode_codecA(str);
}
