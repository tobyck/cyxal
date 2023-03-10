#include <stdbool.h>
#include "../src/parser/tokeniser.h"
#include <wchar.h>

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
}

CyTokenArray *cy_token_array_from(size_t size, CyToken tokens[]) {
    CyTokenArray *array = new_cy_token_array();
    for (int i = 0; i < size; i++) {
        push_cy_token(array, tokens[i]);
    }
    return array;
}

bool run_test(wchar_t *str, size_t size, CyToken exp_tokens[]) {
    CyTokenArray *tokens = tokenise(str);

    CyTokenArray *expected = cy_token_array_from(size, exp_tokens);
    bool result = tokens_eq(tokens, expected);
    if (!result) {
        printf("Got: ");
        print_cy_token_array(tokens);
        printf("\n");
        printf("Expected: ");
        print_cy_token_array(expected);
    }
    free(tokens->tokens);
    free(tokens);
    free(expected->tokens);
    free(expected);
    return result;
}


void test_lexer() {
    int passed = 0;
    int total = 1;
    passed += run_test(L"1 2 +", 3, (CyToken[]){
        { NumberToken, L"1" },
        { NumberToken, L"2" },
        { ElementToken, L"+" }
    });
    printf("Lexer: %d / %d tests passed\n", passed, total);
}