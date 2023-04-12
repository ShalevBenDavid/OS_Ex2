// Created by Shalev Ben David and Ron Shuster.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int copy(FILE*, FILE*, int, char**);

int main (int argc, char* argv[]) {
    //  If there isn't at least 3 arguments, exit.
    if (argc < 3) {
        printf("(-) Not the right format.\n");
        exit(EXIT_FAILURE);
    }
    FILE* file2;
    const char* file_dest = argv[2];
    int exist = access(file_dest, F_OK);

    //  Check if the file already exist or not. 
    if(exist!=-1){
        //  File exists.
        file2 = fopen(argv[2], "a");

    }else{
        // File doesn't exists.
        file2 = fopen(argv[2], "wb");
    }

    // Creating 2 file pointers.
    FILE* file1 = fopen(argv[1], "rb");
    if (!(file1) || !(file2)) {
        printf("(-) Failed to open files.\n");
        fclose(file1);
        fclose(file2);
        exit(EXIT_FAILURE);
    }

    // Calling copy method and saving its return value.
    int return_value = copy(file1, file2, argc, argv);

    // Printing accordingly.
    if (return_value == 1) {
        printf("general failure");
    }
    else {
        printf("success");
    }

    // Closing the file's pointers and returning.
    fclose(file1);
    fclose(file2);
    return return_value;
}

int copy(FILE* file1, FILE* file2, int argc, char** argv){

    fseek(file1, 0, SEEK_SET);
    size_t bytes_read, bytes_written;
    char buffer[1024];
    // While there is more bytes to read -> write them to the secones file.
    while ((bytes_read = fread(buffer,1,sizeof(buffer),file1))>0){
        bytes_written=fwrite(buffer,1,bytes_read,file2);
        if(bytes_written<bytes_read){
            fclose(file1);
            fclose(file2);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}