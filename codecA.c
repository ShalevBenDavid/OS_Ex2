// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include "codec.h"

/**
 * Prints str after encoding it (covert all lower case chars to upper case, and all upper case to lower case).
 * @param str - The string we want to encode.
 */
void encode (char* str) {
    char* temp = str;
    // While we aren't in the end of the string.
    while (*temp) {
        // If it is a lower case, convert to upper case.
        if ((*temp >= 'a') && (*temp <= 'z')) { *temp = *temp - 32; }
        // If it is an upper case, convert to lower case.
        else if((*temp >= 'A') && (*temp <= 'Z')) { *temp = *temp + 32; }
        // Iterate to the next char.
        temp++;
    }
    // Print the string after encoding.
    puts(str);
}

/**
 * Prints str after decoding it (covert all lower case chars to upper case, and all upper case to lower case).
 * For that, we can use encodeA again.
 * @param str - The string we want to decode.
 */
void decode (char* str) {
    // Activating encode again is the same as decoding.
    encode(str);
}