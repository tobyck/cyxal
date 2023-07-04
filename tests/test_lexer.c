#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "../src/parser/lexer.h"

// syntax sugar
#define token_eq(a, b) (a.type == b.type && wcscmp(a.src, b.src) == 0)

// checks if two CyTokenLists are equal
bool tokens_eq(CyTokenList *a, CyTokenList *b) {
	// return false if the sizes are different
	if (a->size != b->size) {
		return false;
	}

	for (int i = 0; i < a->size; ++i) {
		if (!token_eq(a->tokens[i], b->tokens[i])) {
			return false;
		}
	}

	return true;
}

void print_cy_token_list(CyTokenList *tokens) {
	for (int i = 0; i < tokens->size; ++i) {
		wchar_t *stringified_token = stringify_cy_token(tokens->tokens[i]);
		printf("%ls", stringified_token);
		free(stringified_token);
		// print comma if not last token
		if (tokens->size - i > 1) printf(", ");
	}
	printf("\n");
}

CyTokenList *cy_token_list_from(size_t size, CyToken tokens[]) {
	CyTokenList *ret = new_cy_token_list();

	for (int i = 0; i < size; ++i) {
		push_cy_token(ret, tokens[i]);
	}

	return ret;
}

bool run_lexer_test(wchar_t *str, size_t size, CyToken exp_tokens[]) {
	CyTokenList *tokens = lex(str);
	CyTokenList *expected = cy_token_list_from(size, exp_tokens);

	bool result = tokens_eq(tokens, expected);

	if (!result) {
		printf("\nExpected: ");
		print_cy_token_list(expected);
		printf("     Got: ");
		print_cy_token_list(tokens);
	}

	free_cy_token_list(tokens);
	free_cy_token_list(expected);

	return result;
}

void test_lexer() {
	int passed = 0;
	int total = 0;

	passed += run_lexer_test(
		L"1 2 +", 3, (CyToken[]){
			{NumberToken,  L"1"},
			{NumberToken,  L"2"},
			{GeneralToken, L"+"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"`abc\\`def` `hij", 2, (CyToken[]){
			{StringToken, L"`abc\\`def`"},
			{StringToken, L"`hij"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"«abc« »abc»", 2, (CyToken[]){
			{CompressedStringToken, L"«abc«"},
			{CompressedNumberToken, L"»abc»"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"\\a ‛ab \\", 2, (CyToken[]){
			{CharToken,    L"\\a"},
			{TwoCharToken, L"‛ab"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"1 # 2 \n 3", 2, (CyToken[]){
			{NumberToken, L"1"},
			{NumberToken, L"3"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"1 #{#{ 2 }#}# 3 #{ # 4", 2, (CyToken[]){
			{NumberToken, L"1"},
			{NumberToken, L"3"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"←a_B1→cd←", 4, (CyToken[]){
			{VarGetToken, L"←a_B"},
			{NumberToken, L"1"},
			{VarSetToken, L"→cd"},
			{VarGetToken, L"←"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"⁺4 4 ⁺", 2, (CyToken[]){
			{CharNumberToken, L"⁺4"},
			{NumberToken,     L"4"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"1_2.3 4_5", 2, (CyToken[]){
			{NumberToken, L"1_2.3"},
			{NumberToken, L"4_5"}
		}
	);
	total++;

	passed += run_lexer_test(
		L"ka z Þp", 3, (CyToken[]){
			{GeneralToken, L"ka"},
			{GeneralToken, L"z"},
			{GeneralToken, L"Þp"},
		}
	);
	total++;

	printf(
		"%sLexer: %d / %d tests passed            %s%d%%\x1B[0m\n",
		passed == total ? "" : "\n",
		passed,
		total,
		passed == total ? "\x1B[32m" : " \x1B[31m", (int)((double)passed / total * 100)
	);
}