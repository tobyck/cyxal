#include <string.h>
#include "tokeniser.h"
#include "../helpers.h"

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
    // set the value of the last slot to the tokem to add, and increment the size afterwards
    array->tokens[array->size++] = token;
}

// gets the token in a CyTokenArray at the specified index
CyToken get_cy_token(CyTokenArray *array, size_t index) {
    if (index <= array->size) { // if the index is in bounds
        return array->tokens[index]; // return the item
    }
    // otherwise, return an empty token
    CyToken empty_token = {};
    return empty_token;
}

CyTokenArray *tokenise(char *code) {
    CyTokenArray *tokens = new_cy_token_array(); // initialise an empty token array
    LexerState state = ReadyForNext; // set the state as ready for the next token

    for (int i = 0; i < strlen(code); i++) {
        const char c = code[i];

        if (state == ReadyForNext) { // if we're ready for the next token
            if (contains(DIGITS_WITH_DEC, c)) { // if the char is a digit (or a dec. place)
                CyToken token = { NumberToken, str_from_chr(c) }; // create a new number token with the number so far which is stored in src
                push_cy_token(tokens, token); // push that token to the token array
                if (i < strlen(code) - 1) { // if we're not at the last char
                    if (contains(DIGITS_WITH_DEC, code[i + 1])) { // and the next char is also a digit
                        state = NumberState; // set the state to NumberState to keep building the current token
                    }
                }
            }
        } else {
            if (state == NumberState) {
                append_str(tokens->tokens[tokens->size - 1].src, str_from_chr(c)); // dynamically append the char to the token's src
                // set the state back to ready for next if the next character isn't a digit
                if (i < strlen(code) - 1) {
                    if (
                        !contains(DIGITS_WITH_DEC, code[i + 1])
                        || (
                            code[i + 1] == DEC_PLACE
                            && contains(tokens->tokens[tokens->size - 1].src, DEC_PLACE)
                        )
                    ) {
                        state = ReadyForNext;
                    }
                }
            }
        }
    }

    return tokens;
}