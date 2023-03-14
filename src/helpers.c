#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include <wchar.h>
#include <stdbool.h>

// checks if a string contains a certain char
bool contains(wchar_t *str, wchar_t c) {
    if (wcschr(str, c) != NULL) {
        return true;
    }
    return false;
}

// for a heap-allocated string, dynamically concatenate another string
void append_str(wchar_t *dest, wchar_t *src) {
    dest = realloc(dest, (wcsnlen(dest, 1024) + wcsnlen(src, 1024)) * sizeof(wchar_t));
    wcscat(dest, src);
}

// create a null-terminated string from a char
wchar_t *str_from_chr(wchar_t c) {
    wchar_t *str = malloc(2*sizeof(wchar_t));
    swprintf(str, 1024, L"%lc", c);
    return str;
}

wchar_t *str_to_wcs(char *c) {
    size_t size = strlen(c) + 1;
    wchar_t *wc = malloc(size / sizeof(char) * sizeof(wchar_t));
    mbstowcs (wc, c, size);
    return wc;
}

char *wcs_to_str(wchar_t *wc) {
    size_t size = wcsnlen(wc, 1024) + 1;
    char *c = malloc(size);
    sprintf (c, "%ls", wc);
    return c;
}