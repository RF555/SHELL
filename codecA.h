#ifndef CODEC_A_H
#define CODEC_A_H

#include <stdio.h>

/**
 * Convert all lower case chars to upper case, and all upper case to lower case. All other chars will remain unchanged.
 * @param message Message to encode.
 * @param message_len Length of the message.
 * @param encoded Pointer to the encode.
 */
void encode_codecA(char *message, int message_len, char *encoded);

/**
 * Decode the string back to it's original form.
 * @param message Message to decode.
 * @param message_len Length of the message.
 * @param encoded Pointer to the decode.
 */
void decode_codecA(char *message, int message_len, char *encoded);

#endif


