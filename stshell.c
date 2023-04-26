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
        exit(EXIT_FAILURE);
    }

    // Creating a file pointer to the file to the right of the sign with the appropriate permission.
    FILE* file = fopen(argv[index + 1], permission);
    if (!file) {
        perror("(-) Failed to open file.\n");
        exit(EXIT_FAILURE);
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
    // If this is the parent process, wait for the child process to terminate and return exit status.
    wait(NULL);
}

// A function that execute redirect command.
void pipe_execute (char** argv, int pipe_count, int redirect1, int redirect2) {
    // If there is no file to output to in case of redirect, then print error.
    if (!argv[redirect1 + 1] || !argv[redirect2 + 1]) {
        perror("(-) Redirect Failed. Missing file.\n");
        exit(EXIT_FAILURE);
    }

    // In case of redirection, open the output file in the appropriate permission.
    if (redirect1 != -1 || redirect2 != -1) {
        FILE* file = NULL;
        if (redirect1 != -1)
            file = fopen(argv[redirect1 + 1], "w");
        else
            file = fopen(argv[redirect2 + 1], "a");
        if (!file) {
        perror("(-) Failed to open file.\n");
        exit(EXIT_FAILURE);
        }
    // Obtain the file descriptor.
    int fd_output = fileno(file);
    }

    //Create a Matrix 2x2 (size 4) of file descriptors.
    int fd[2][2];

    for (int i = 0; i < pipe_count; i++) {
        // Create a pipe.
        if (pipe(fd[i]) == -1) {
            perror("(-) Failed to create pipe.\n");
            exit(EXIT_FAILURE);
        }
    }

    int index = 0;
    for (int i = 0; i <= pipe_count; i++) {
        // Create new array to store command till pipe appears.
        int new_index = 0;
        char* left_command[256];
        while (argv[index]) {
            // If we encountered a pipe, break loop.
            if (!strcmp(argv[index], "|"))
                break;
            // Copy command.
            left_command[new_index++] = argv[index];
            index++;
        }
        left_command[new_index] = NULL;
        // iterate over the pipe.
        index++;
        //printf("%d %s", index, *left_command);
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
            if (i != 0)
                // Redirect the output.
                dup2(fd[i][0], STDIN_FILENO);
            if (i != pipe_count) {
                // Redirect the output.
                dup2(fd[i][1], STDOUT_FILENO);
            }
            // Close pipes.
            close(fd[i][0]);
            close(fd[i][1]);
            // Run the left command.
            execvp(left_command[0], left_command);
            // If command failed.
            perror("(-) Failed to execute command.\n");
            exit(EXIT_FAILURE);
        }
        else
            wait(NULL);
        // Close pipes.
        for (int i = 0; i < pipe_count; i++) {
            for (int j = 0; j <= 1; j++)
                close(fd[i][j]);
        }

    }
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
            // If this is the parent process, wait for the child process to terminate and return exit status.
            wait(NULL);
        }
    }
}