// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>

// ****************** Encode A String ******************
void encodeB (char* str) {
    char* temp = str;
    // While we aren't in the end of the string.
    while (*temp) {
        // Add 3 to the ascii value.
        *temp += 3;
        // Iterate to the next char.
        temp++;
    }
    // Print the string after encoding.
    puts(str);
}

// ****************** Decode A String ******************
void decodeB (char* str) {
    char* temp = str;
    // While we aren't in the end of the string.
    while (*temp) {
        // Subtract 3 from the ascii value.
        *temp += 3;
        // Iterate to the next char.
        temp++;
    }
    // Print the string after encoding.
    puts(str);
}