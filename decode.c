// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char* argv[]) {
    // If there isn't exactly 3 arguments, exit.
    if (argc != 3) {
        printf("(-) Not the right format.\n");
        exit(EXIT_FAILURE);
    }

    // Save the library name.
    char* lib_name = (char*) calloc(8 + strlen(argv[1]), sizeof(char));
    if (!(lib_name)) {
        printf("(-) Failed to save the library name.\n");
        free(lib_name);
        exit(EXIT_FAILURE);
    }
    strcat(lib_name, "./lib");
    strcat(lib_name, argv[1]);
    strcat(lib_name, ".so");

    // Load the dynamic library.
    void* lib = dlopen(lib_name, RTLD_NOW);
    free(lib_name);
    // Print in case of an error.
    if (!(lib)) {
        printf("(-) Error in opening library: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // The encode function pointer.
    void (*decode)(char*) = dlsym(lib, "decode");
    // Print in case of an error.
    if (!(decode)) {
        printf("(-) Error in linking encode function: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // Calling the encode function.
    decode(argv[2]);

    // Closing the library. Printing in case of an error.
    if (dlclose(lib)) {
        printf("(-) Error in closing library: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    // Return 0 upon success.
    return 0;
}