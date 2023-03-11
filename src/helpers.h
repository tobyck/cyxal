#ifndef CYXAL_HELPERS_H
#define CYXAL_HELPERS_H

#include <stdbool.h>

// constants

#define DIGITS "0123456789"
#define DEC_PLACE '.'
#define DIGITS_WITH_DEC "0123456789."
#define DIGRAPHS "k∆øÞ¨"

// useful functions

extern bool contains(char *str, char c);
extern void append_str(char *dest, char *src);
extern char *str_from_chr(char str);

#endif // CYXAL_HELPERS_H
