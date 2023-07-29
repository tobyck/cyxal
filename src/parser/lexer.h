#ifndef CYXAL_LEXER_H
#define CYXAL_LEXER_H

#include <stdbool.h>
#include <wchar.h>

// token related types

typedef enum {
	NumberToken,
	GeneralToken, // Elements / modifiers / structures / whatever. The parser deals with this
	StringToken,
	CompressedStringToken,
	CompressedNumberToken,
	VarSetToken,
	VarGetToken,
	CharToken,
	TwoCharToken,
	CharNumberToken,
	NewlineToken
} CyTokenType;

typedef struct {
	CyTokenType type;
	wchar_t *src;
} CyToken;

// methods for CyTokens

extern CyToken new_cy_token(CyTokenType type, wchar_t *src);
extern wchar_t *stringify_cy_token(CyToken token);

// token list struct

typedef struct {
	CyToken *tokens; // pointer to the start of the list - the first token
	size_t size; // the current size of the list
} CyTokenList;

// methods for CyTokenLists

extern CyTokenList *new_cy_token_list(void);
extern void push_cy_token(CyTokenList *list, CyToken token);
extern void free_cy_token_list(CyTokenList *list, bool free_src);

// the lex function

extern CyTokenList *lex(wchar_t *code);

#endif // CYXAL_LEXER_H
