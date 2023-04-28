// Created by Shalev Ben David and Ron Shuster.
#include <sys/wait.h>
#include "stdio.h"
#include <stdlib.h>
#include "unistd.h"
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>


// A function which is called whenever the user enters ctrl + C to stop running tool.
void signal_handler() {
    printf(" User pressed Ctrl + C. If you want to exit shell, enter \"exit\".\n");
}

// A function that returns the number of pipes.
int pipe_counter (char** argv) {
    int i = 0;
    int save = 0;
    while (argv[i]) {
        // If it is a pipe.
        if (!strcmp(argv[i], "|"))
            save++;
        i++;
    }
    return save;
}

// A function that looks for a sign and returns its index.
int find_sign (char** argv, char* c) {
    int i = 0;
    while (argv[i]) {
        if (!strcmp(argv[i], c))
            return i;
        i++;
    }
    return -1;
}

// A function that execute redirect command.
void redirect_execute (char** argv, char* permission, int index) {
    // If there is no file to output to, then print error.
    if (!argv[index + 1]) {
        perror("(-) Redirect Failed. Missing file.\n");
        return;
    }

    // Creating a file pointer to the file to the right of the sign with the appropriate permission.
    FILE* file = fopen(argv[index + 1], permission);
    if (!file) {
        perror("(-) Failed to open file.\n");
        return;
    }
    // Obtain the file descriptor.
    int fd = fileno(file);

    // Save the left command (command till the ">").
    char* left_command[index + 1];
    for (int j = 0; j < index; j++) {
        left_command[j] = argv[j];
    }
    left_command[index] = NULL;

    // Use fork to create a new process and save the process id.
    pid_t pid = fork();
    // If fork failed, print it.
    if (pid == -1) {
        perror("(-) Fork command failed. %s\n");
    }
    // If this is the child, execute command accordingly.
    else if (pid == 0) {
        // Don't ignore signals (like ctrl+c).
        signal(SIGINT, SIG_DFL);
        // Redirect the standard output to our file.
        dup2(fd, STDOUT_FILENO);
        // Close the file descriptor.
        fclose(file);
        // Run the left command.
        execvp(left_command[0], left_command);
        // If command failed.
        perror("(-) Failed to execute command.");
        exit(EXIT_FAILURE);
    }
    // If this is the parent process, wait for the child process to terminate
    wait(NULL);
}

// A function that execute redirect command.
void pipe_execute (char** argv, int pipe_count, int redirect1, int redirect2) {
    // If there is no file to output to in case of redirect, then print error.
    if ((redirect1 != -1 && !argv[redirect1 + 1]) || (redirect2 != -1 && !argv[redirect2 + 1])) {
        perror("(-) Redirect Failed. Missing file.\n");
        return;
    }

    // In case of redirection, open the output file in the appropriate permission.
    int fd_redirect = -1;
    if (redirect1 != -1 || redirect2 != -1) {
        FILE *file = NULL;
        if (redirect1 != -1) {
            file = fopen(argv[redirect1 + 1], "w");
            argv[redirect1] = NULL;
        } else {
            file = fopen(argv[redirect2 + 1], "a+");
            argv[redirect2] = NULL;
        }
        if (!file) {
            perror("(-) Failed to open file.\n");
            return;
        }
        // Obtain the file descriptor.
        fd_redirect = fileno(file);
    }

    // Create a Matrix 2x2 (size 4) of file descriptors.
    int pipes[2][2];
    for (int i = 0; i < pipe_count; i++) {
        // Create a pipe.
        if (pipe(pipes[i]) == -1) {
            perror("(-) Failed to create pipe.\n");
            exit(EXIT_FAILURE);
        }
    }

    int index = 0;

    for (int i = 0; i <= pipe_count; i++) {
        char *command[1024];
        int temp_index = 0;
        while (argv[index] != NULL && strcmp(argv[index], "|")) {
            if (index == redirect1 || index == redirect2)
                index++;
            command[temp_index++] = argv[index++];
        }
        // End command with null.
        command[temp_index] = NULL;
        // Skip the pipeline "|".
        index++;

        pid_t pid = fork();
        if (pid == 0) {
            if (i == 1)
                dup2(pipes[0][0], STDIN_FILENO);
            if (i == 2)
                dup2(pipes[1][0], STDIN_FILENO);
            if (i != pipe_count){
                if (i == 0)
                    dup2(pipes[0][1], STDOUT_FILENO);
                else
                    dup2(pipes[1][1], STDOUT_FILENO);
            }
            // Else, if there is redirection.
            else if (fd_redirect != -1)
                dup2(fd_redirect, STDOUT_FILENO);

            // Close pipes.
            for (int j = 0; j < pipe_count; j++)
                for (int k = 0; k <= 1; k++)
                    close(pipes[j][k]);

            // If there was redirect, close his file descriptor.
            if (fd_redirect != -1)
                close(fd_redirect);

            // Execute command.
            execvp(command[0], command);
            perror("Error executing command");
            exit(EXIT_FAILURE);
        }
    }
    // Close pipes.
    for (int j = 0; j < pipe_count; j++)
        for (int k = 0; k <= 1; k++)
            close(pipes[j][k]);

    if (fd_redirect != -1)
        close(fd_redirect);

    // Wait for the child process to terminate.
    for (int i = 0; i <= pipe_count; i++)
        wait(NULL);
}


int main() {
    int i;
    char* argv[256];
    char command[1024];
    char* token;

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

        int redirect1 = find_sign(argv, ">");
        int redirect2 = find_sign(argv, ">>");
        int pipe_count = pipe_counter(argv);

        // Case 1: > or >> or none AND | or ||.
        if (pipe_count) {
            pipe_execute(argv, pipe_count, redirect1, redirect2);
        }
        // Case 2: > or >>.
        else if (redirect1 != -1 || redirect2 != -1) {
            if (redirect1 != -1)
                redirect_execute(argv, "w", redirect1);
            else
                redirect_execute(argv, "a", redirect2);
        }
        // No special sign. Execute command.
        else {
            // Use fork to create a new process and save the process id.
            pid_t pid = fork();
            // If fork failed, print it.
            if (pid == -1) {
                perror("(-) Fork command failed.\n");
            }
            // If this is the child, execute command accordingly.
            else if (pid == 0) {
                // Don't ignore signals (like ctrl+c).
                signal(SIGINT, SIG_DFL);
                // Run the command.
                execvp(argv[0], argv);
                // If command failed.
                perror("(-) Failed to execute command.\n");
                exit(EXIT_FAILURE);
            }
            // If this is the parent process, wait for the child process to terminate.
            wait(NULL);
        }
    }
}