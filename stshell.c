// Created by Shalev Ben David and Ron Shuster.
#include <sys/wait.h>
#include "stdio.h"
#include <stdlib.h>
#include "unistd.h"
#include <string.h>
#include <stdbool.h>

// A function which is called whenever the user enters ctrl + C to stop running tool.
void signal_handler() {
    printf(" User pressed Ctrl + C. If you want to exit shell, enter \"exit\".\n");
};

int main() {
    int i;
    char *argv[256];
    char command[1024];
    char *token;

    // Ignore signals (like ctrl+c).
    signal(SIGINT, signal_handler);

    // Endless loop.
    while (true) {

        //The shell name.
        printf("\033[35mstshell@ShalevAndShuster:\033[0m ");
        // Receive command from user.
        fgets(command, 1024, stdin);
        // replace \n with \0
        command[strlen(command) - 1] = '\0';

        // Insert command into argv and separate by spaces.
        i = 0;
        token = strtok (command," ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok (NULL, " ");
            i++;
        }
        // Last argument is null.
        argv[i] = NULL;

        // User didn't enter anything.
        if (argv[0] == NULL)
            continue;

        // If user wants to exit shell.
        if (strcmp(argv[0], "exit") == 0) {
            return EXIT_SUCCESS;
        }

        // Use fork to create a new process and save the process id.
        pid_t pid = fork();

        // If fork failed, print it.
        if (pid == -1) {
            perror("(-) Fork command failed. %s\n");
        }
        // If this is the child, execute command.
        else if (pid == 0) {
            // Don't ignore signals (like ctrl+c).
            signal(SIGINT, SIG_DFL);

            // User wants to redirect and overwrite with ">".
            if (strcmp(argv[i - 2], ">") == 0) {
                // Save the left command (command till the ">").
                char* left_command[i - 2];
                for (int j = 0; j < i - 2; j++) {
                    left_command[j] = argv[j];
                }
                left_command[i - 2] = NULL;
                // Creating a file pointer to arv[i-1] in write mode.
                FILE* file = fopen(argv[i - 1], "w");
                if (!file) {
                    perror("(-) Failed to open file.\n");
                    exit(EXIT_FAILURE);
                }
                // Obtain the file descriptor.
                int fd = fileno(file);
                // Redirect the standard output to our file.
                dup2(fd, STDOUT_FILENO);
                // Close the file descriptor.
                fclose(file);
                // Run the left command.
                execvp(left_command[0], left_command);
                // If command failed.
                perror("(-) Error in executing command.");
                exit(EXIT_FAILURE);
            }
            else if (strcmp(argv[i - 2], ">>") == 0) {
                // Save the left command (command till the ">").
                char *left_command[i - 2];
                for (int j = 0; j < i - 2; j++) {
                    left_command[j] = argv[j];
                }
                left_command[i - 2] = NULL;
                // Creating a file pointer to argb[i-1] in append mode.
                FILE* file = fopen(argv[i - 1], "a");
                if (!file) {
                    perror("(-) Failed to open file.\n");
                    exit(EXIT_FAILURE);
                }
                // Obtain the file descriptor.
                int fd = fileno(file);
                // Redirect the standard output to our file.
                dup2(fd, STDOUT_FILENO);
                // Close the file descriptor.
                fclose(file);
                // Run the left command.
                execvp(left_command[0], left_command);
                // If command failed.
                perror("(-) Error in executing command.");
                exit(EXIT_FAILURE);
            }
            else {
                execvp(argv[0], argv);
                // If command failed.
                perror("(-) Error in executing command.");
                exit(EXIT_FAILURE);
            }
        }
        // If this is the parent process, wait for the child process to terminate and return exit status.
        wait(NULL);
    }
}