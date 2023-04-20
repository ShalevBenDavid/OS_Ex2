// Created by Shalev Ben David and Ron Shuster.
#include <sys/wait.h>
#include "stdio.h"
#include <stdlib.h>
#include "unistd.h"
#include <string.h>
#include <stdbool.h>

int main() {
    int i;
    char *argv[10];
    char command[1024];
    char *token;

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
            return 0;
        }

        // Ignore signals (like ctrl+c).
        signal(SIGINT, SIG_IGN);

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
//            if (strcmp(argv[1], ">") == 0) {
//                // Creating a file pointer to argv[2].
//                FILE* file = fopen(argv[2], "w");
//                if (!file) {
//                    perror("(-) Failed to open file.\n");
//                    exit(EXIT_FAILURE);
//                }
//                // Obtain the file descriptor.
//                int fd = fileno(file);
//                // Redirect the standard output to out file.
//                dup2(STDOUT_FILENO, fd);
//                // Run the command.
//                execv(argv[0], argv);
//            }
            execvp(argv[0], argv);
        }
        // If this is the parent process, wait for the child process to terminate and return exit status.
        wait(NULL);
    }
}
