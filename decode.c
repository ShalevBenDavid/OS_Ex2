// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "codec.h"

int main(int argc, char* argv[]) {
    // If there isn't exactly 3 arguments, exit.
    if (argc != 3) {
        fprintf("(-) Not the right format.\n");
        exit(EXIT_FAILURE);
    }

    // Load the dynamic library.
    void *lib = dlopen(strcat("./", strcat(argv[1], ".so")), RTLD_NOW);
    // Print in case of an error.
    if (!(lib)) {
        fprintf(stderr, "(-) Error in opening library: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // The encode function pointer.
    void (*decode)(char*) = dlsym(lib, "decode");
    // Print in case of an error.
    if (!(decode)) {
        fprintf(stderr, "(-) Error in linking encode function: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // Calling the encode function.
    decode(argv[2]);

    // Closing the library. Printing in case of an error.
    if (dlclose(lib)) {
        fprintf(stderr, "(-) Error in closing library: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // Return 0 upon success.
    return 0;
}