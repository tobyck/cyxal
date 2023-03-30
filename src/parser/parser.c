#include "parser.h"
#include <stddef.h>
#include <stdlib.h>
#include "lexer.h"

#define new_ast(tag, ...) heap_alloc_ast((AST){tag, {.tag=(struct tag){__VA_ARGS__}}})

AST *heap_alloc_ast(AST ast) {
    AST *ptr = malloc(sizeof(AST));
    *ptr = ast;
    return ptr;
}




void ast_test() {
    AST *test_ast = new_ast(AST_STATEMENT, L"1");
    //AST *test2 = heap_alloc_ast((AST){ AST_NUMBER, (struct AST_NUMBER) {L"1"}});
}

AST *parse(CyTokenArray *tokens) {
    AST *structures = new_ast(AST_GENERIC, new_ast(AST_NULL), 0)
    for (int i = 0; i < tokens->size; i++) {
        CyToken token = tokens->tokens[i];
        CyTokenType type = token.type;
        if (type == ConstantToken || type == VarToken) {

        }
    }
}