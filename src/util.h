#ifndef CYXAL_UTIL_H
#define CYXAL_UTIL_H

#include <stdbool.h>

#define DIGITS "0123456789."
#define DEC_PLACE '.'

extern bool contains(char *str, char c);
extern void append_str(char *dest, char *src);
extern char *str_from_chr(char str);

#endif // CYXAL_UTIL_H
