// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include <stdlib.h>

int compare (FILE*, FILE*, int, char*);

int main (int argc, char* argv[]) {
    //  If there isn't at least 3 arguments, exit.
    if (argc < 3) {
        printf("(-) Not the right format.\n");
        exit(EXIT_FAILURE);
    }

    // Creating 2 file pointers.
    FILE* file1 = fopen(argv[1], "r");
    FILE* file2 = fopen(argv[2], "r");
    if (!(file1) || !(file2)) {
        printf("(-) Failed to open files.\n");
        exit(EXIT_FAILURE);
    }

    // Calling compare method and returning its return value.
    return compare(file1, file2, argc, *argv);
}

int compare (FILE* file1, FILE* file2, int argc, char* argv) {
    if (argc == 4) {
        puts(&argv[3]);
    }

    // Setting the file's indexes to the start.
    fseek(file1, 0, SEEK_SET);
    fseek(file2, 0, SEEK_SET);

    // Initialize 2 chars to the start of each file.
    char c1 = fgetc(file1);
    char c2 = fgetc(file2);

    // While we didn't finished both files, compare them.
    while ((c1 != EOF) && (c2 != EOF)) {
        if (c1 != c2) {
            return EXIT_FAILURE;
        }
        // Get next char in each file.
        c1 = fgetc(file1);
        c2 = fgetc(file2);
    }
    // Check both file ended in the same time.
    if (c1 == EOF && c2 == EOF) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
