#include "lexer.h"
#include "../helpers.h"
#include <stdbool.h>

// returns a new empty token array
CyTokenArray *new_cy_token_array(void) {
    CyTokenArray *array = malloc(sizeof(CyTokenArray));
    array->tokens = malloc(0); // don't actually allocate any memory but initialise the pointer
    array->size = 0; // set the array's size to 0
    return array;
}

// appends a new token to a CyTokenArray
void push_cy_token(CyTokenArray *array, CyToken token) {
    // reallocate memory so there's one more "slot" in the array
    array->tokens = realloc(array->tokens, sizeof(CyToken) * (array->size + 1));
    // set the value of the last slot to the token to add, and increment the size afterwards
    array->tokens[array->size++] = token;
}

wchar_t *stringify_cy_token(CyToken token) {
    wchar_t *str = malloc(50 * sizeof(wchar_t));
    swprintf(str, 50 * sizeof(wchar_t), L"{ type: %d, src: \"%ls\" }", token.type, token.src);
    return str;
}

// NOTE: This should only split strings into their tokens, *not* do any postprocessing with them.

CyTokenArray *lex(wchar_t *code) {
    CyTokenArray *tokens = new_cy_token_array(); // initialise an empty token array

    for (int i = 0; i < wcslen(code); i++) {
        wchar_t c = code[i];
        wchar_t *c_as_str = chr_to_str(c);

        if (contains(DIGITS_WITH_DEC, c)) { // if the char is a digit (or a dec. place)
            push_cy_token(tokens, (CyToken) { NumberToken, c_as_str });
            i++; // consume current char
            bool has_dec = c == DEC_PLACE;
            for (; i < wcslen(code) && (contains(DIGITS_WITH_DEC, code[i]) || code[i] == '_'); i++) {
                if (c == DEC_PLACE) {
                    if (has_dec) break;
                    has_dec = true;
                }
                append_str(&tokens->tokens[tokens->size - 1].src, chr_to_str(code[i]));
            }
            if (i < wcslen(code)) i--; // if not eof shift back to the last char
        // if the char is a digraph modifier or single-char element
        } else if (contains(DIGRAPHS, c)) {
            // add a token with the current char
            push_cy_token(tokens, (CyToken){ GeneralToken, c_as_str });
            if (contains(DIGRAPHS, c) && i < wcslen(code) - 1) { // if the char is a digraph, and it's not the last char
                // append the next char in the code to the last token and go to the next loop iteration
                append_str(&tokens->tokens[tokens->size - 1].src, chr_to_str(code[++i]));
            }
        } else if (c == COMPRESSED_STR_DEL || c == COMPRESSED_NUM_DEL) {
            i++; // consume start
            for (; i < wcslen(code) && code[i] != c; i++) append_str(&c_as_str, chr_to_str(code[i]));
            if (i < wcslen(code)) append_str(&c_as_str, chr_to_str(c)); // append start
            push_cy_token(tokens, (CyToken) {c == COMPRESSED_STR_DEL ? CompressedStringToken : CompressedNumberToken, c_as_str});
        } else if (c == CHAR_DELIMITER) {
            if (i < wcslen(code) - 1) {// If at least 1 char left
                append_str(&c_as_str,
                           chr_to_str(code[++i])); // it's okay to do this since string is never modified again
                push_cy_token(tokens, (CyToken) {CharToken, c_as_str});
            }
        } else if (c == DOUBLE_CHAR_STR && i < wcslen(code) - 2) {
            append_str(&c_as_str, chr_to_str(code[++i]));
            append_str(&c_as_str, chr_to_str(code[++i]));
            push_cy_token(tokens, (CyToken) {TwoCharToken, c_as_str});
        } else if (c == COMMENT) {
            if (i < wcslen(code) - 1 && code[i + 1] == OPEN_BLOCK_COMMENT) {
                int depth = 1; // comment depth
                i += 2; // consume #{
                for (; i < wcslen(code); i++) {
                    if (code[i] == COMMENT && i < wcslen(code) - 1 && code[i + 1] == OPEN_BLOCK_COMMENT) {
                        i++; // consume #
                        depth++;
                    } else if (code[i] == CLOSE_BLOCK_COMMENT && i < wcslen(code) - 1 && code[i + 1] == COMMENT){
                        i++; // consume }
                        depth--;
                        if (depth == 0) {
                            break;
                        }
                    }
                }
            } else {
                for (; i < wcslen(code) && code[i] != NEWLINE; i++); // move forward until newline or eof
            }
        } else if (c == GET_VAR || c == SET_VAR) {
            push_cy_token(tokens, (CyToken) {c == GET_VAR ? VarGetToken : VarSetToken, c_as_str});
            i++; // consume var char
            for (; i < wcslen(code) && (isalpha(code[i]) || code[i] == L'_'); i++) {
                append_str(&tokens->tokens[tokens->size - 1].src, chr_to_str(code[i]));
            }
            if (i < wcslen(code)) i--; // If not eof, shift the pointer back to the last char
        } else if (c == CHAR_NUMBER) {
            if (i < wcslen(code) - 1) {
                append_str(&c_as_str, chr_to_str(code[++i]));
                push_cy_token(tokens, (CyToken) {CharNumberToken, c_as_str});
            }
        } else if (c == STRING_DELIMETER) {
            i++; // consume `
            for (; i < wcslen(code) && code[i] != STRING_DELIMETER; i++) {
                append_str(&c_as_str, chr_to_str(code[i]));
                if (code[i] == ESCAPE_CHAR && i < wcslen(code) - 1) { // if escape, advance pointer without checking
                    append_str(&c_as_str, chr_to_str(code[++i]));
                }
            }
            if (i < wcslen(code)) append_str(&c_as_str, chr_to_str(STRING_DELIMETER));
            push_cy_token(tokens, (CyToken) {StringToken, c_as_str});
        } else if (c == NEWLINE) {
            push_cy_token(tokens, (CyToken) {NewlineToken, c_as_str}); // for lambda to newline
        } else if (c != SPACE) {
            // We don't care if it's actually an element or not. (checking only for elements causes issues)
            // This is for structures, elements, modifiers - anything that doesn't have a multichar source.
            push_cy_token(tokens, (CyToken) { GeneralToken, c_as_str});
        }
    }

    return tokens;
}