#include <wchar.h>
#include <stddef.h>
#include <gmp.h>

#ifndef CYXAL_PARSER_H
#define CYXAL_PARSER_H

// recursive type kludge
typedef struct AST AST;

struct AST {
    enum {
        AST_MONADIC_MODIFIER,
        AST_DYADIC_MODIFIER,
        AST_ELEMENT,
        AST_IF_STATEMENT,
        AST_FOR_LOOP,
        AST_WHILE_LOOP,
        AST_LAMBDA,
        AST_LIST,
        AST_GENERIC,
        AST_STATEMENT,
        AST_NULL
    } tag;
    union {
        struct AST_MONADIC_MODIFIER {AST *node; wchar_t *str; } AST_MONADIC_MODIFIER;
        struct AST_DYADIC_MODIFIER {AST *node; wchar_t *str; } AST_DYADIC_MODIFIER;
        struct AST_IF_STATEMENT { AST *true_case; AST *false_case; } AST_IF_STATEMENT;
        struct AST_FOR_LOOP { wchar_t *var; AST *body; } AST_FOR_LOOP;
        struct AST_WHILE_LOOP { AST *cond; AST *body; } AST_WHILE_LOOP;
        struct AST_LAMBDA { AST *body;  wchar_t *type; } AST_LAMBDA;
        struct AST_LIST { AST *list; size_t size; } AST_LIST;
        struct AST_GENERIC { AST *body; size_t size; } AST_GENERIC;
        struct AST_STATEMENT { wchar_t *value; } AST_STATEMENT;
        struct AST_NULL {} AST_NULL;
    } data;
};





/*
// modifiers.c
// dynamically linked at runtime
void single_element_lambda (func func_01, Context ctx) {
    CyValue* values = ctx.stack.popMultiple(func_01.arity.in);
    ctx.push_stack(empty_stack())
    ctx.stack.pushMultiple(values);
    func_01(ctx);
    CyValue* new_values = ctx.stack.popMultiple(func_01.arity.out);
    ctx.pop_stack();
    ctx.stack.pushMultiple(new_values);
}


void func_01 (Context ctx) {
    reduce_by(double_elem, ctx)
}

void func_02 (Context ctx) {
    single_element_lambda(func_01, ctx);
}

push_value(*func_02)
*/
#endif // CYXAL_PARSER_H