#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // These two are only here to prevent warnings when compiling.
#include <sys/wait.h>
#include "tokenize.c" // Used to separate the arguments in the input string.

/// @brief Prompts the user for a string less than a given length.
/// @param askstr The field where the input string will be stored.
/// @param length The maximum length of the input string.
/// @return The string input by the user.
char *progask(char *askstr, int length) {
    printf("> "); // For the sake of clarity.
    fgets(askstr, sizeof(char)*length, stdin); // Get the string from stdin.
    return askstr;
}

/// @brief Creates a child process and runs a program with arguments provided by the user.
/// @param argc The number of arguments by the user.
/// @param argv The list of arguments by the user.
void execute(int argc, char **argv) {
    int pid;
    int err;
    pid = fork(); // Create a child process with pid 0.
    if (pid == 0) { // Child process.
        err = execv(argv[0], argv); // If the program runs successfully, this should not return.
        printf("An error has occurred: ");
        perror("");
        exit(err); // Exit with error.
    }
    else { // Parent process.
        int status = wait(NULL); // Wait for the child process to end before continuing.
        if (status == -1) { // If no child process was created, provide a helpful error.
            printf("Something went wrong while executing that command.\n");
        }
    }
}

/// @brief A miniature-scale version of a UNIX shell, capable of running programs with arguments and exiting.
/// @param argc The number of arguments provided by the user.
/// @param argv The list of arguments provided by the user.
/// @return Exit status.
int main(int argc, char **argv)
{
    int tcount = 10; // These two values define the maximum length and number of parameters.
    int strlen = 80;
    int tlen = (strlen/tcount)*2+1;
    char *str; // Pointer to the input string.
    char **tenv; // Template parameter vector with the maximum possible space.
    char **fenv; // Temporary vector returned by the tokenize function.
    int status = 1; // This will be used to detect blank input and the "exit" command.
    tenv = malloc(tcount*sizeof(char**)); // Allocate space for template vector.
    if (tenv == NULL) {
        printf("Failed to allocate memory.");
        exit(1);
    }
    for (int i=0; i<tcount; i++) {
        tenv[i] = malloc(tlen*sizeof(char*));
        if (tenv[i] == NULL) {
            printf("Failed to allocate memory.");
            exit(1);
        }
    }
    str = (char*) malloc(sizeof(char)*strlen); // Allocate space for input string.
    if (str == NULL) {
        printf("Failed to allocate memory.");
        exit(1);
    }
    while (status != 0) {
    progask(str, strlen); // Prompt user for input.
    fenv = tokenize(str, &tcount, tenv); // Tokenize input string.
    status = strcmp(fenv[0],"exit"); // Check for "exit".
    if (status != 0) {
        status = strcmp(fenv[0],""); // Check for blank input.
        if (status != 0) {
            int tokens = 0; // Count the number of tokens in the temporary vector.
            for (int i=0; i<strlen; i++) {
                if (fenv[i]!=NULL) {
                    tokens++;
                }
                str[i]=0; // Blank the input string via the same loop.
            }
            execute(tokens, fenv); // Execute the requested program.
        }
        status = -1; // Reset status.
    }
        free(fenv); // Reset temporary vector.
    }
    free(str); // Free allocated memory before exiting.
    free(tenv);
    exit(0); // Exit normally.
}