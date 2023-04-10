// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>

// ****************** Encode A String ******************
void encodeA (char* str) {
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

// ****************** Decode A String ******************
void decodeA (char* str) {
    // Activating encode again is the same as decoding.
    encode(str);
}