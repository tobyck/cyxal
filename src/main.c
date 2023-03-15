/*
 * Purely for testing at this point
 * Most of this code should be organised and moved to the tests folder
 * */

#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include "parser/lexer.h"
#include "builtins/cy_value.h"
#include "builtins/elements.h"
#include "context.h"
#include "../tests/test_lexer.c"

void test_lexer_input() {
    wchar_t *input = malloc(128);
    printf("code: ");
    fgetws(input, 128, stdin);
    input[wcscspn(input, L"\n")] = 0;

    CyTokenArray *tokens = lex(input);

    for (int i = 0; i < tokens->size; i++) {
        printf("%ls\n", stringify_cy_token(tokens->tokens[i]));
    }

    free(input);
    free(tokens->tokens);
    free(tokens);
}

void test_cy_value() {
    CyValue *list1 = cy_value_new_list(empty_cy_value_list());
    push_cy_value(list1, *cy_value_new_str(L"abc"));

    CyValue *list2 = cy_value_new_list(empty_cy_value_list());
    push_cy_value(list2, *list1);
    push_cy_value(list2, *cy_value_new_num(L"3.2"));

    CyValue *list3 = cy_value_new_list(empty_cy_value_list());
    push_cy_value(list3, *cy_value_new_num(L".625"));
    push_cy_value(list3, *cy_value_new_func(L"n+H"));
    push_cy_value(list3, *list2);

    printf("%ls\n", stringify_cy_value(*list3));
}

void test_context() {
    CyContext *ctx = new_cy_context(empty_cy_value_list(), L"");
    printf("%ls\n", stringify_cy_value(*halve(ctx, cy_value_new_num(L"12.3"))));
}

void test_elements() {
    CyElementList *elements = get_elements();
    wchar_t **symbols = elements_symbols(elements);
    for (int i = 0; i < elements->size; i++) {
        printf("%ls\n", symbols[i]);
    }
}

int main() {
    setlocale(LC_ALL, "");

    test_lexer();

    return 0;
}