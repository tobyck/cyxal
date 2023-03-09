/*
 * Purely for testing at this point
 * */

#include <stdio.h>
#include "parser/tokeniser.h"
#include "builtins/cy_value.h"

char *stringify_cy_token(CyToken token) {
    char *str = malloc(64);
    sprintf(str, "{ type: %d, src: \"%s\" }", token.type, token.src);
    return str;
}

void test_lexer() {
    char *input = malloc(16);
    printf("code: ");
    fgets(input, 256, stdin);
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
    CyValue *value1 = new_cy_value(NumberType);
    cy_value_set_num(value1, "1234");

    CyValue *value2 = new_cy_value(StringType);
    cy_value_set_str(value2, "Hello");

    CyValueList *list1 = new_cy_value_array();
    push_cy_value(list1, *value1);
    push_cy_value(list1, *value2);

    CyValueList *list2 = new_cy_value_array();
    push_cy_value(list2, *value2);

    CyValue *list_value2 = new_cy_value(ListType);
    list_value2->other = list2;

    push_cy_value(list1, *list_value2);

    CyValue *list_value1 = new_cy_value(ListType);
    list_value1->other = list1;

    printf("%s\n", stringify_cy_value(*list_value1));

    free_cy_value_list(list1);
    free_cy_value_list(list2);
}

int main() {
    test_cy_value();

    return 0;
}