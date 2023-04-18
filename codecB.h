#ifndef CODEC_B_H
#define CODEC_B_H


// * @param str String to encode.
// */
/**
 * Convert all chars to the 3-rd next char (adding a number of 3 to the ASCII value).
 * @param message Message to encode.
 * @param message_len Length of the message.
 * @param encoded Pointer to the encode.
 */
void encode_codecB(char *message, int message_len, char *encoded);

/**
 * Decode the string back to it's original form.
 * @param message Message to decode.
 * @param message_len Length of the message.
 * @param encoded Pointer to the decode.
 */
void decode_codecB(char *message, int message_len, char *encoded);

#endif
