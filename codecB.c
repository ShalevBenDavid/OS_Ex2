// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include "codec.h"

/**
 * Prints str after encoding it (convert all chars to the 3-rd next char).
 * @param str - The string we want to encode.
 */
void encode (char* str) {
    char* temp = str;
    // While we aren't in the end of the string.
    while (*temp) {
        // Add 3 to the ascii value.
        *temp = (*temp + 3) % 127;
        // Iterate to the next char.
        temp++;
    }
    // Print the string after encoding.
    puts(str);
}

/**
 * Prints str after decoding it (decrease by 3 the ascii value of each char).
 * @param str  - The string we want to decode.
 */
void decode (char* str) {
    char* temp = str;
    // While we aren't in the end of the string.
    while (*temp) {
        // Subtract 3 from the ascii value.
        *temp = (*temp - 3) % 127;
        // Iterate to the next char.
        temp++;
    }
    // Print the string after encoding.
    puts(str);
}