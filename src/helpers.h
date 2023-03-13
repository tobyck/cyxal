#ifndef CYXAL_HELPERS_H
#define CYXAL_HELPERS_H

#include <stdbool.h>

// constants

#define DIGITS "0123456789"
#define DEC_PLACE '.'
#define DIGITS_WITH_DEC "0123456789."
#define DIGRAPHS "k∆øÞ¨"
#define COMPRESSED_STR_DEL L'«'
#define COMPRESSED_NUM_DEL L'»'
#define STRING_DELIMETER L'`'
#define NUMBER_DELIMETER L'_'
#define DOUBLE_CHAR_STR L'‛'
#define CHAR_DELIMITER L'\\'
#define COMMENT L'#'
#define NEWLINE L'\n'
#define SPACE L' '
#define OPEN_STRUCT L"[({⟨λƛ⟑'µ"
#define CLOSE_STRUCT L")}]⟩;"
#define LAMBDA_TO_NEWLINE L")"


// useful functions

extern bool contains(char *str, char c);
extern void append_str(char *dest, char *src);
extern char *str_from_chr(char str);

#endif // CYXAL_HELPERS_H
