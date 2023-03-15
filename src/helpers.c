#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>
#include "helpers.h"

// checks if a string contains a certain char
bool contains(wchar_t *str, wchar_t chr) {
    if (wcschr(str, chr) != NULL) {
        return true;
    }
    return false;
}

void append_str(wchar_t **dest, wchar_t *src) {
    *dest = realloc(*dest, (wcslen(*dest) + wcslen(src) + 1) * sizeof(wchar_t));
    wcscat(*dest, src);
}

// create a null-terminated string from a char
wchar_t *chr_to_str(wchar_t chr) {
    wchar_t *str = malloc(2 * sizeof(wchar_t));
    swprintf(str, 2, L"%lc", chr);
    return str;
}

// convert a standard string (char *) to a wide char string (4 bytes per char)
wchar_t *str_to_wcs(char *str) {
    size_t size = strlen(str) + 1; // + 1 for null byte
    wchar_t *wcs = malloc(size * sizeof(wchar_t));
    mbstowcs (wcs, str, size);
    return wcs;
}

// convert a wide char string to a normal string
char *wcs_to_str(wchar_t *wcs) {
    char *c = malloc(wcslen(wcs) + 1);
    sprintf (c, "%ls", wcs);
    return c;
}