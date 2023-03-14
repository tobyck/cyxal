#ifndef CYXAL_HELPERS_H
#define CYXAL_HELPERS_H

#include <stdbool.h>

// constants

#define DIGITS L"0123456789"
#define DEC_PLACE L'.'
#define DIGITS_WITH_DEC L"0123456789."
#define DIGRAPHS L"k∆øÞ¨"
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

extern bool contains(wchar_t *str, wchar_t c);
extern void append_str(wchar_t *dest, wchar_t *src);
extern wchar_t *str_from_chr(wchar_t str);
extern wchar_t *str_to_wcs(char *c);
extern char *wcs_to_str(wchar_t *wc);


#endif // CYXAL_HELPERS_H
