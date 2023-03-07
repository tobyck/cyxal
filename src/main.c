#include <stdio.h>
#include "lexer.h"

int main() {
    char *input = malloc(16);
    printf("code: ");
    fgets(input, 256, stdin);
    input[strcspn(input, "\n")] = 0;

    CyTokenArray *tokens = lex(input);

    for (int i = 0; i < tokens->size; i++) {
        printf("%s\n", stringifyCyToken(tokens->tokens[i]));
    }

    free(input);
    free(tokens->tokens);
    free(tokens);

    return 0;
}