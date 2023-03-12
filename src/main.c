/*
 * Purely for testing at this point
 * */

#include <stdio.h>
#include "parser/tokeniser.h"
#include "builtins/cy_value.h"
#include "builtins/elements.h"
#include "context.h"

char *stringify_cy_token(CyToken token) {
    char *str = malloc(64);
    sprintf(str, "{ type: %d, src: \"%s\" }", token.type, token.src);
    return str;
}

void test_lexer() {
    char *input = malloc(128);
    printf("code: ");
    fgets(input, 128, stdin);
    input[strcspn(input, "\n")] = 0;

    CyTokenArray *tokens = tokenise(input);

    for (int i = 0; i < tokens->size; i++) {
        printf("%s\n", stringify_cy_token(tokens->tokens[i]));
    }

    free(input);
    free(tokens->tokens);
    free(tokens);
}

void test_cy_value() {
    CyValue *list1 = cy_value_new_list(empty_cy_value_list());
    push_cy_value(list1, *cy_value_new_str("abc"));

    CyValue *list2 = cy_value_new_list(empty_cy_value_list());
    push_cy_value(list2, *list1);
    push_cy_value(list2, *cy_value_new_num("3.2"));

    CyValue *list3 = cy_value_new_list(empty_cy_value_list());
    push_cy_value(list3, *cy_value_new_num(".625"));
    push_cy_value(list3, *cy_value_new_func("n+H"));
    push_cy_value(list3, *list2);

    printf("%s\n", stringify_cy_value(*list3));
}

void test_context() {
    CyContext *ctx = new_cy_context(empty_cy_value_list(), "");
    printf("%s\n", stringify_cy_value(*halve(ctx, cy_value_new_num("12.3"))));
}

void test_elements() {
    CyElementList *elements = get_elements();
    char **symbols = elements_symbols(elements);
    for (int i = 0; i < elements->size; i++) {
        printf("%s\n", symbols[i]);
    }
}

int main() {
    return 0;
}