#include <stdio.h>
#include "tokeniser.h"

// returns a new empty token array
CyTokenArray *new_cy_token_array() {
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

// checks if a string contains a certain char
bool contains(char *str, char c) {
    if (strchr(str, c) != NULL) {
        return true;
    }
    return false;
}

// for a heap-allocated string, dynamically concatenate another string
void append_str(char *dest, char *src) {
    dest = realloc(dest, (strlen(dest) + strlen(src)) * sizeof(char));
    strcat(dest, src);
}

// create a null-terminated string from a char
char *str_from_chr(char c) {
    char *str = malloc(2);
    sprintf(str, "%c", c);
    return str;
}

CyTokenArray *tokenise(char *code) {
    CyTokenArray *tokens = new_cy_token_array(); // initialise an empty token array
    LexerState state = ReadyForNext; // set the state as ready for the next toke

    for (int i = 0; i < strlen(code); i++) {
        const char c = code[i];

        if (state == ReadyForNext) { // if we're ready for the next token
            if (contains(DIGITS, c)) { // if the char is a digit (or a dec. place)
                CyToken token = { NumberToken, str_from_chr(c) }; // create a new number token with the number so far which is stored in src
                push_cy_token(tokens, token); // push that token to the token array
                if (i < strlen(code) - 1) { // if we're not at the last char
                    if (contains(DIGITS, code[i + 1])) { // and the next char is also a digit
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
                        !contains(DIGITS, code[i + 1])
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