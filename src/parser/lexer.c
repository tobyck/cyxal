#include <stdlib.h>
#include "lexer.h"
#include "../helpers.h"

// creates a new CyToken from a type and a source string
CyToken new_cy_token(CyTokenType type, wchar_t *src) {
	wchar_t *src_copy = malloc((wcslen(src) + 1) * sizeof(wchar_t));
	wcscpy(src_copy, src);
	return (CyToken){type, src_copy};
}

// creates a string from a CyToken in the format { type: <type>, src: "<src>" }
wchar_t *stringify_cy_token(CyToken token) {
	wchar_t *str = malloc(0);
	append_str(&str, L"{ type: ");

	wchar_t *type = malloc(3 * sizeof(wchar_t));
	swprintf(type, 3, L"%d", token.type);

	append_str(&str, type);
	append_str(&str, L", src: \"");
	append_str(&str, token.src);
	append_str(&str, L"\" }");

	return str;
}

// returns a new empty token array
CyTokenList *new_cy_token_list(void) {
	CyTokenList *list = malloc(sizeof(CyTokenList));
	list->tokens = malloc(0);
	list->size = 0;
	return list;
}

// appends a new token to a CyTokenList
void push_cy_token(CyTokenList *list, CyToken token) {
	// reallocate memory so there's one more "slot" in the list
	list->tokens = realloc(list->tokens, sizeof(CyToken) * (list->size + 1));
	// set the value of the last slot to the token to add, and increment the size afterwards
	list->tokens[list->size++] = token;
}

void free_cy_token_list(CyTokenList *list, bool free_src) {
	// if the src attribute of each token should be freed
	if (free_src) {
		for (int i = 0; i < list->size; ++i) {
			free(list->tokens[i].src);
		}
	}

	free(list->tokens);
	free(list);
}

// NOTE: This should only split strings into their tokens, *not* do any postprocessing with them.

CyTokenList *lex(wchar_t *code) {
	CyTokenList *tokens = new_cy_token_list(); // initialise an empty token array

	for (int i = 0; i < wcslen(code); ++i) {
		wchar_t c = code[i];
		wchar_t *token_src = chr_to_wcs(c);

		if (contains(DIGITS_WITH_DEC, c)) { // if the char is a digit (or a dec. place)
			push_cy_token(tokens, new_cy_token(NumberToken, token_src));
			i++; // consume current char
			bool has_dec = c == DEC_PLACE;
			for (; i < wcslen(code) && (contains(DIGITS_WITH_DEC, code[i]) || code[i] == L'_'); ++i) {
				if (c == DEC_PLACE) {
					if (has_dec) break;
					has_dec = true;
				}
				append_str_and_free(&tokens->tokens[tokens->size - 1].src, chr_to_wcs(code[i]));
			}
			if (i < wcslen(code)) i--; // if not eof shift back to the last char
			// if the char is a digraph modifier or single-char element
		} else if (contains(DIGRAPHS, c)) {
			// add a token with the current char
			push_cy_token(tokens, new_cy_token(GeneralToken, token_src));
			if (contains(DIGRAPHS, c) && i < wcslen(code) - 1) { // if the char is a digraph, and it's not the last char
				// append the next char in the code to the last token and go to the next loop iteration
				append_str_and_free(&tokens->tokens[tokens->size - 1].src, chr_to_wcs(code[++i]));
			}
		} else if (c == COMPRESSED_STR_DEL || c == COMPRESSED_NUM_DEL) {
			i++; // consume start
			for (; i < wcslen(code) && code[i] != c; ++i) append_str_and_free(&token_src, chr_to_wcs(code[i]));
			if (i < wcslen(code)) append_str_and_free(&token_src, chr_to_wcs(c)); // append start
			push_cy_token(
				tokens, new_cy_token(c == COMPRESSED_STR_DEL ? CompressedStringToken : CompressedNumberToken, token_src)
			);
		} else if (c == CHAR_DELIMITER) {
			if (i < wcslen(code) - 1) { // if at least 1 char left
				append_str_and_free(
					&token_src, chr_to_wcs(code[++i])); // it's okay to do this since string is never modified again
				push_cy_token(tokens, new_cy_token(CharToken, token_src));
			}
		} else if (c == DOUBLE_CHAR_STR && i < wcslen(code) - 2) {
			append_str_and_free(&token_src, chr_to_wcs(code[++i]));
			append_str_and_free(&token_src, chr_to_wcs(code[++i]));
			push_cy_token(tokens, new_cy_token(TwoCharToken, token_src));
		} else if (c == COMMENT) {
			if (i < wcslen(code) - 1 && code[i + 1] == OPEN_BLOCK_COMMENT) {
				int depth = 1; // comment depth
				i += 2; // consume #{
				for (; i < wcslen(code); ++i) {
					if (code[i] == COMMENT && i < wcslen(code) - 1 && code[i + 1] == OPEN_BLOCK_COMMENT) {
						i++; // consume #
						depth++;
					} else if (code[i] == CLOSE_BLOCK_COMMENT && i < wcslen(code) - 1 && code[i + 1] == COMMENT) {
						i++; // consume }
						depth--;
						if (depth == 0) {
							break;
						}
					}
				}
			} else {
				for (; i < wcslen(code) && code[i] != NEWLINE; ++i); // move forward until newline or eof
			}
		} else if (c == GET_VAR || c == SET_VAR) {
			push_cy_token(tokens, new_cy_token(c == GET_VAR ? VarGetToken : VarSetToken, token_src));
			i++; // consume var char
			for (; i < wcslen(code) && (isalpha(code[i]) || code[i] == L'_'); ++i) {
				append_str_and_free(&tokens->tokens[tokens->size - 1].src, chr_to_wcs(code[i]));
			}
			if (i < wcslen(code)) i--; // If not eof, shift the pointer back to the last char
		} else if (c == CHAR_NUMBER) {
			if (i < wcslen(code) - 1) {
				append_str_and_free(&token_src, chr_to_wcs(code[++i]));
				push_cy_token(tokens, new_cy_token(CharNumberToken, token_src));
			}
		} else if (c == STRING_DELIMETER) {
			i++; // consume `
			for (; i < wcslen(code) && code[i] != STRING_DELIMETER; ++i) {
				append_str_and_free(&token_src, chr_to_wcs(code[i]));
				if (code[i] == ESCAPE_CHAR && i < wcslen(code) - 1) { // if escape, advance pointer without checking
					append_str_and_free(&token_src, chr_to_wcs(code[++i]));
				}
			}
			if (i < wcslen(code)) {
				// I have absolutely no idea why you have to free chr_to_wcs(STRING_DELIMETER) here instead
				// of just using the append_str_and_free helper but it works so I'm just gonna accept it.
				wchar_t *tmp = chr_to_wcs(STRING_DELIMETER);
				append_str(&token_src, tmp);
				free(tmp);
			}
			push_cy_token(tokens, new_cy_token(StringToken, token_src));
		} else if (c == NEWLINE) {
			push_cy_token(tokens, new_cy_token(NewlineToken, token_src)); // for lambda to newline
		} else if (c != SPACE) {
			// We don't care if it's actually an element or not. (checking only for elements causes issues)
			// This is for structures, elements, modifiers - anything that doesn't have a multichar source.
			push_cy_token(tokens, new_cy_token(GeneralToken, token_src));
		}

		free(token_src);
	}

	return tokens;
}