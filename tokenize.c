#include <stdio.h>
#include <stdlib.h>
#include <string.h> // These two are only here to prevent warnings when compiling.
#include <ctype.h>

/// @brief Converts an input string into a list of strings delimited by whitespace.
/// @param tokenstr Input string.
/// @param tokencnt Maximum number of tokens to detect.
/// @param tokenvec Parameter vector to base the return vector on.
/// @return Vector containing a list of strings.
char **tokenize(char *tokenstr, int *tokencnt, char **tokenvec) {
    int pos = 0; // Position in current token.
    int ovpos = 0; // Overall position within string.
    long lastpos = (long) tokenstr; // Pointer to the start of the token.
    int tokens = 0; // Number of tokens.
    char **newvec = malloc((tokens+1)*sizeof(char*)); // Vector to be generated.
    while (tokenstr[ovpos] != 0 && tokens<*tokencnt) { // End on null terminator or maximum tokens.
            if (isspace(tokenstr[ovpos])) { // Keep moving until whitespace is found.
                newvec[tokens] = malloc(sizeof(tokenvec[tokens])); // Allocate token slot.
                *newvec[tokens] = *tokenvec[tokens]; // Copy token space from template vector.
                strncpy(newvec[tokens], (char*) lastpos, pos); // Copy token to vector.
                while (isspace(tokenstr[ovpos+1])) { // For additional whitespace, keep moving.
                    ovpos++;
                    pos++;
                }
                lastpos = lastpos + pos + 1; // Set position of next token.
                pos = -1; // Reset current position in token.
                tokens++; // Increment number of tokens.
            }
            ovpos++;
            pos++;
    }
    newvec[tokens] = NULL; // Add null terminator to the end of the new vector.
    return newvec;
}