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
            }
        } else {
            if (state == NumberState) {
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
            }
        }
    }

    return tokens;
}