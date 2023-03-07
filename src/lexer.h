#ifndef CYXAL_LEXER_H
#define CYXAL_LEXER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// token related types

typedef enum {
    NumberToken
} CyTokenType;

typedef enum {
    ReadyForNext,
    NumberState
} LexerState;

typedef struct {
    CyTokenType type;
    char *src;
} CyToken;

// TEMP
char *stringifyCyToken(CyToken token);

// token array

typedef struct {
    CyToken *tokens; // pointer to the start of the array - the first token
    size_t size; // the current size of the array
} CyTokenArray;

CyTokenArray *newCyTokenArray();
void pushCyToken(CyTokenArray *array, CyToken token);
CyToken getCyToken(CyTokenArray *array, size_t index);

// the lex function

CyTokenArray *lex(char *code);

// misc

#define DIGITS "0123456789."
bool contains(char *str, char c);
void appendStr(char *dest, char *src);
char *strFromChar(char str);

#endif // CYXAL_LEXER_H
