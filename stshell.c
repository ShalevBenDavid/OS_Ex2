// Created by Shalev Ben David and Ron Shuster.
#include <sys/wait.h>
#include "stdio.h"
#include "unistd.h"
#include <string.h>
#include <stdbool.h>

int main() {
    int i;
    char *argv[10];
    char command[1024];
    char *token;

    while (true) {
        printf("stshell: ");
        fgets(command, 1024, stdin);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* parse command line */
        i = 0;
        token = strtok (command," ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok (NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL)
            continue;

        /* for commands not part of the shell command language */
        if (fork() == 0) {
            execvp(argv[0], argv);
        }
        // For the parent process.
        wait(NULL);

    }
}
