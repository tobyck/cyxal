#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

// checks if a string contains a certain char
bool contains(char *str, char c) {
    if (strchr(str, c) != NULL) {
        return true;
    }
    return false;
}

// for a heap-allocated string, dynamically concatenate another string
void append_str(char *dest, char *src) {
    dest = realloc(dest, (strlen(dest) + strlen(src)) * sizeof(char));
    strcat(dest, src);
}

// create a null-terminated string from a char
char *str_from_chr(char c) {
    char *str = malloc(2);
    sprintf(str, "%c", c);
    return str;
}