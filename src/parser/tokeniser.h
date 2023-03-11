#ifndef CYXAL_TOKENISER_H
#define CYXAL_TOKENISER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// token related types

typedef enum {
    NumberToken,
    ElementToken
} CyTokenType;

typedef enum {
    ReadyForNext,
    NumberState,
    ElementState
} LexerState;

typedef struct {
    CyTokenType type;
    char *src;
} CyToken;

// token array struct

typedef struct {
    CyToken *tokens; // pointer to the start of the array - the first token
    size_t size; // the current size of the array
} CyTokenArray;

// array methods

extern CyTokenArray *new_cy_token_array(void);
extern void push_cy_token(CyTokenArray *array, CyToken token);
extern CyToken get_cy_token(CyTokenArray *array, size_t index);

// the tokenise function

extern CyTokenArray *tokenise(char *code);

#endif // CYXAL_TOKENISER_H
