// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BYTES 1024

int copy(FILE*, FILE*);

int main (int argc, char* argv[]) {
    //  If there isn't at least 3 arguments, exit.
    if (argc < 3) {
        printf("(-) Not the right format. The format is \"./copy <file1> <file2> -v -f\" where"
               " v and f flags are optional.\n");
        return EXIT_FAILURE;
    }

    // Initialize 2 ints to hold the flags.
    bool v_flag = false;
    bool f_flag = false;

    // Checking if the user set one or both of the flags.
    for (int i = 0; i < argc; i++) {
        if (strcmp("-v", argv[i]) == 0) {
            if (i < 3) {
                printf("(-) Not the right format. The format is \"./copy <file1> <file2> -v -f\" where"
                       " v and f flags are optional.\n");
                return EXIT_FAILURE;
            }
            v_flag = true;
        }
        if (strcmp("-f", argv[i]) == 0) {
            if (i < 3) {
                printf("(-) Not the right format. The format is \"./copy <file1> <file2> -v -f\" where"
                       " v and f flags are optional.\n");
                return EXIT_FAILURE;
            }
            f_flag = true;
        }
    }

    // Getting the target file's name.
    const char* target_name = argv[2];

    // Creating the target file pointer.
    FILE* file2;

    // Open the target file based on the f flag and its existence.
    int exist = access(target_name, F_OK);
    if ((exist == 0 && f_flag == true) || (exist == -1)) {
        // File doesn't exist or exist and user wants to overwrite. Create in write mode.
        file2 = fopen(argv[2], "wb");
    } else {
        // File exists. Append to end.
        file2 = fopen(argv[2], "ab");
    }

    // Creating the source file pointer.
    FILE* file1 = fopen(argv[1], "rb");
    if (!(file1) || !(file2)) {
        printf("(-) Failed to open files.\n");
        exit(EXIT_FAILURE);
    }

    // Calling copy method and saving its return value.
    int return_value = copy(file1, file2);

    // Printing accordingly.
    if (exist == 0 && v_flag == true) {
        printf("target file exist\n");
    }
    if (return_value == 1 && v_flag == true) {
        printf("general failure\n");
    }
    else if(return_value == 0 && v_flag == true){
        printf("success\n");
    }

    // Closing the file's pointers and returning.
    fclose(file1);
    fclose(file2);
    return return_value;
}

int copy(FILE* file1, FILE* file2){

    // Initialize variables.
    size_t bytes_read = 0, bytes_written = 0;
    char buffer[MAX_BYTES] = {0};

    // Setting the source file index to the start.
    fseek(file1, 0, SEEK_SET);

    // While there is more bytes to read -> write them to the second file.
    while ((bytes_read = fread(buffer, sizeof(char), MAX_BYTES, file1)) > 0) {
        bytes_written = fwrite(buffer, sizeof(char), bytes_read, file2);
        // If we wrote less than we read, there is an error.
        if (bytes_written < bytes_read) {
            fclose(file1);
            fclose(file2);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}