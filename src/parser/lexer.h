#ifndef CYXAL_TOKENISER_H
#define CYXAL_TOKENISER_H

#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

// token related types

typedef enum {
    NumberToken,
    ElementToken,
    StringToken,
    CompressedStringToken,
    CompressedNumberToken,
    VarSetToken,
    VarGetToken
} CyTokenType;

typedef enum {
    ReadyForNext,
    NumberState,
    ElementState,
    StringState,
    EscapedStringState,
    CompressedStringState,
    CompressedNumberState
} LexerState;

typedef struct {
    CyTokenType type;
    wchar_t *src;
} CyToken;

// token array struct

typedef struct {
    CyToken *tokens; // pointer to the start of the array - the first token
    size_t size; // the current size of the array
} CyTokenArray;

// methods for CyTokenArrays and CyTokens

extern CyTokenArray *new_cy_token_array(void);
extern void push_cy_token(CyTokenArray *array, CyToken token);
extern wchar_t *stringify_cy_token(CyToken token);

// the lex function

extern CyTokenArray *lex(wchar_t *code);

#endif // CYXAL_TOKENISER_H
