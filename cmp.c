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
    if (compare(file1, file2, argc, *argv) == 1) {
        printf("distinct");
    }
    else {
        printf("equal");
    }
}

int compare (FILE* file1, FILE* file2, int argc, char* argv) {

    // Setting the file's indexes to the start.
    fseek(file1, 0, SEEK_SET);
    fseek(file2, 0, SEEK_SET);

    // Creating 2 chars to iterate over each file.
    char c1 = 0;
    char c2 = 0;

    // While we didn't finished both files, compare them.
    while (((c1 = fgetc(file1)) != EOF) || ((c2 = fgetc(file2)) != EOF)) {
        if (c1 != c2) { return EXIT_FAILURE; }
    }
    // Check both file ended in the same time.
    if (c1 == EOF && c2 == EOF) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
