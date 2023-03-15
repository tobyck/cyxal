#include "lexer.h"
#include "../helpers.h"
#include "../builtins/elements.h"

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

CyTokenArray *lex(wchar_t *code) {
    CyTokenArray *tokens = new_cy_token_array(); // initialise an empty token array
    LexerState state = ReadyForNext; // set the state as ready for the next token

    CyElementList *elements = get_elements();

    for (int i = 0; i < wcslen(code); i++) {
        wchar_t c = code[i];
        wchar_t *c_as_str = chr_to_str(c);

        if (state == ReadyForNext) { // if we're ready for the next token
            if (contains(DIGITS_WITH_DEC, c)) { // if the char is a digit (or a dec. place)
                CyToken token = { NumberToken, c_as_str }; // create a new number token with the number so far which is stored in src
                push_cy_token(tokens, token); // push that token to the token array
                if (i < wcslen(code) - 1) { // if we're not at the last char
                    if (contains(DIGITS_WITH_DEC, code[i + 1])) { // and the next char is also a digit
                        state = NumberState; // set the state to NumberState to keep building the current token
                    }
                }
            // if the char is a digraph modifier or single-char element
            } else if (contains(DIGRAPHS, c) || has_element(elements, c_as_str)) {
                // add a token with the current char
                push_cy_token(tokens, (CyToken){ ElementToken, c_as_str });
                if (contains(DIGRAPHS, c) && i < wcslen(code) - 1) { // if the char is a digraph, and it's not the last char
                    // append the next char in the code to the last token and go to the next loop iteration
                    append_str(&tokens->tokens[tokens->size - 1].src, chr_to_str(code[++i]));
                }
            } else if (c == STRING_DELIMETER) {
                state = StringState;
                push_cy_token(tokens, (CyToken) {StringToken, chr_to_str(STRING_DELIMETER)});
            } else if (c == COMPRESSED_STR_DEL) {
                state = CompressedStringState;
                push_cy_token(tokens, (CyToken) {CompressedStringToken, chr_to_str(COMPRESSED_STR_DEL)});
            } else if (c == COMPRESSED_NUM_DEL) {
                state = CompressedNumberState;
                push_cy_token(tokens, (CyToken) {CompressedNumberToken, chr_to_str(COMPRESSED_NUM_DEL)});
            } else if (c == CHAR_DELIMITER && i < wcslen(code) - 1) { // If at least 1 char left
                append_str(&c_as_str, chr_to_str(code[++i])); // it's okay to do this since string is never modified again
                push_cy_token(tokens, (CyToken) {StringToken, c_as_str});
            } else if (c == DOUBLE_CHAR_STR && i < wcslen(code) - 2) {
                append_str(&c_as_str, chr_to_str(code[++i]));
                append_str(&c_as_str, chr_to_str(code[++i]));
                push_cy_token(tokens, (CyToken) {StringToken, c_as_str});
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
                                state = ReadyForNext;
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
            }
        } else if (state == NumberState) {
            append_str(&tokens->tokens[tokens->size - 1].src, c_as_str); // dynamically append the char to the token's src
            // set the state back to ready for next if the next character isn't a digit
            if (i < wcslen(code) - 1) {
                if (
                    !contains(DIGITS_WITH_DEC, code[i + 1]) // if the next char isn't a digit
                    || ( // or the next char is a decimal place but there already is one
                        code[i + 1] == DEC_PLACE
                        && contains(tokens->tokens[tokens->size - 1].src, DEC_PLACE)
                    )
                ) {
                    state = ReadyForNext; // set the state back to ReadyForNext to tokenise the next token
                }
            }
        } else if (state == StringState) {
            if (c == STRING_DELIMETER) {
                state = ReadyForNext;
            }
            if (c == ESCAPE_CHAR) {
                state = EscapedStringState;
            }
            append_str(&tokens->tokens[tokens->size - 1].src, c_as_str);
        } else if (state == EscapedStringState) {
            state = StringState;
            append_str(&tokens->tokens[tokens->size - 1].src, c_as_str);
        } else if (state == CompressedStringState) {
            if (c == COMPRESSED_STR_DEL) {
                state = ReadyForNext;
            }
            append_str(&tokens->tokens[tokens->size - 1].src, c_as_str);
        } else if (state == CompressedNumberState) {
            if (c == COMPRESSED_NUM_DEL) {
                state = ReadyForNext;
            }
            append_str(&tokens->tokens[tokens->size - 1].src, c_as_str);
        }
    }

    return tokens;
}