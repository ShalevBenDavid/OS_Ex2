// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // If there isn't exactly 3 arguments, exit.
    if (argc != 3) {
        exit(EXIT_FAILURE);
    }
    // If the user choose to encode using codecA library.
    if (strcmp(argv[1], "codecA") == 0) {
        decodeA(argv[2]);
    }
    // If the user choose to encode using codecA library.
    else if (strcmp(argv[1], "codecB") == 0) {
        decodeB(argv[2]);
    }
    // Else the input is invalid, so exit.
    else {
        exit(EXIT_FAILURE);
    }
    return 0;
}