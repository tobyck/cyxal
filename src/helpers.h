#ifndef CYXAL_HELPERS_H
#define CYXAL_HELPERS_H

#include <stdbool.h>
#include <wchar.h>

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
#define ESCAPE_CHAR L'\\'
#define OPEN_BLOCK_COMMENT L'{'
#define CLOSE_BLOCK_COMMENT L'}'
#define GET_VAR L'←'
#define SET_VAR L'→'
#define CHAR_NUMBER L'⁺'
#define CODEPAGE L"λƛ¬∧⟑∨⟇÷×«\n»°•ß†€½∆ø↔¢⌐æʀʁɾɽÞƈ∞¨ !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]`^_abcdefghijklmnopqrstuvwxyz{|}~↑↓∴∵›‹∷¤ð→←βτȧḃċḋėḟġḣḭŀṁṅȯṗṙṡṫẇẋẏż√⟨⟩‛₀₁₂₃₄₅₆₇₈¶⁋§ε¡∑¦≈µȦḂĊḊĖḞĠḢİĿṀṄȮṖṘṠṪẆẊẎŻ₌₍⁰¹²∇⌈⌊¯±₴…□↳↲⋏⋎꘍ꜝ℅≤≥≠⁼ƒɖ∪∩⊍£¥⇧⇩ǍǎǏǐǑǒǓǔ⁽‡≬⁺↵⅛¼¾Π„‟"

// useful functions

extern bool contains(wchar_t *str, wchar_t chr);
extern wchar_t *append_str(wchar_t **dest, wchar_t *src);
extern wchar_t *chr_to_str(wchar_t chr);
extern wchar_t *str_to_wcs(char *str);
extern char *wcs_to_str(wchar_t *wcs);
extern bool is_letter(wchar_t wc);

#endif // CYXAL_HELPERS_H
