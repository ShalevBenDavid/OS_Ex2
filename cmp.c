// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int compare (FILE*, FILE*, bool);

int main (int argc, char* argv[]) {
    //  If there isn't at least 3 arguments, exit.
    if (argc < 3) {
        printf("(-) Not the right format.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize boolean to indicate flags.
    bool v_flag = false;
    bool i_flag = false;

    // Checking if the user set one or both of the flags.
    for (int i = 0; i < argc; i++){
        if (strcmp("-v", argv[i]) == 0) { v_flag = true; }
        if (strcmp("-i", argv[i]) == 0){ i_flag = true; }
    }

    // Creating 2 file pointers.
    FILE* file1 = fopen(argv[1], "r");
    FILE* file2 = fopen(argv[2], "r");
    if (!(file1) || !(file2)) {
        printf("(-) Failed to open files.\n");
        exit(EXIT_FAILURE);
    }

    // Calling compare method and saving its return value.
    int return_value = compare(file1, file2, i_flag);

    // Printing accordingly.
    if (return_value == 1 && v_flag==1) {
        printf("distinct\n");
    }
    else if(return_value==0 && v_flag==1){
        printf("equal\n");
    }

    // Closing the file's pointers and returning.
    fclose(file1);
    fclose(file2);
    return return_value;
}

int compare (FILE* file1, FILE* file2, bool i_flag) {

    // Setting the file's indexes to the start.
    fseek(file1, 0, SEEK_SET);
    fseek(file2, 0, SEEK_SET);

    // Initialize 2 chars to the start of each file.
    wchar_t c1 = getc(file1);
    wchar_t c2 = getc(file2);

    // While we didn't finished both files, compare them.
    while ((c1 != EOF) && (c2 != EOF)) {
        // Ignore case scenario.
        if (i_flag == true) {
            if((c1 >= 'A') && (c1 <= 'Z')) { c1 += 32; }
            if((c2 >= 'A') && (c2 <= 'Z')) { c2 += 32; }
        }
        if (c1 != c2) { return EXIT_FAILURE; }
        // Get the next char in each file.
        c1 = getc(file1);
        c2 = getc(file2);
    }

    // Check both file ended in the same time.
    if ((c1 == EOF) && (c2 == EOF)) { return EXIT_SUCCESS; }
    else { return EXIT_FAILURE; }
}