#ifndef CODEC_B_H
#define CODEC_B_H

/**
 * Convert all chars to the 3-rd next char (adding a number of 3 to the ASCII value).
 * @param str String to encode.
 */
void encode_codecB(char *str);

/**
 * Decode the string back to it's original form.
 * @param str - String to decode.
 */
void decode_codecB(char *str);

#endif
