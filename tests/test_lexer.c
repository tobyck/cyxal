#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "../src/parser/lexer.h"

// syntax sugar

#define token_eq(a, b) (a.type == b.type && wcscmp(a.src, b.src) == 0)

bool tokens_eq(CyTokenArray *a, CyTokenArray *b) {
	if (a->size != b->size) {
		return false;
	}
	for (int i = 0; i < a->size; i++) {
		if (!token_eq(a->tokens[i], b->tokens[i])) {
			return false;
		}
	}
	return true;
}

void print_cy_token_array(CyTokenArray *tokens) {
	for (int i = 0; i < tokens->size; i++) {
		printf("%ls", stringify_cy_token(tokens->tokens[i]));
		if (tokens->size - i > 1) printf(", ");
	}
	printf("\n");
}

CyTokenArray *cy_token_array_from(size_t size, CyToken tokens[]) {
	CyTokenArray *array = new_cy_token_array();
	for (int i = 0; i < size; i++) {
		push_cy_token(array, tokens[i]);
	}
	return array;
}

bool run_lexer_test(wchar_t *str, size_t size, CyToken exp_tokens[]) {
	CyTokenArray *tokens = lex(str);

	CyTokenArray *expected = cy_token_array_from(size, exp_tokens);
	bool result = tokens_eq(tokens, expected);
	if (!result) {
		printf("\nExpected: ");
		print_cy_token_array(expected);
		printf("     Got: ");
		print_cy_token_array(tokens);
	}
	free(tokens->tokens);
	free(tokens);
	free(expected->tokens);
	free(expected);
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