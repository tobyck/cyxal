#include <stdlib.h>
#include <wchar.h>
#include "context.h"

CyContext *new_cy_context(CyValueList *args) {
	CyContext *ctx = malloc(sizeof(CyContext));
	ctx->args = args;
	ctx->stacks = empty_stack_list();
	push_empty_stack(ctx);
	ctx->output = malloc(0);
	ctx->error = malloc(0);
	return ctx;
}

void free_cy_context(CyContext *ctx) {

}

StackList *empty_stack_list(void) {
	StackList *list = malloc(sizeof(StackList));
	list->stacks = malloc(0);
	list->size = 0;
	return list;
}

void push_empty_stack(CyContext *ctx) {
	ctx->stacks->stacks = realloc(ctx->stacks->stacks, (ctx->stacks->size + 1) * sizeof(CyValueList));
	ctx->stacks->stacks[ctx->stacks->size++] = empty_cy_value_list();
}

CyValueList *last_stack(CyContext *ctx) {
	return ctx->stacks->stacks[ctx->stacks->size - 1];
}

CyValueList *pop_stack(CyContext *ctx) {
	CyValueList *ret = last_stack(ctx);
	ctx->stacks->stacks = realloc(ctx->stacks->stacks, (--ctx->stacks->size) * sizeof(CyValueList));
	return ret;
}

CyValue *pop_arg(CyContext *ctx) {
	return pop_cy_value(last_stack(ctx));
}

CyValueList pop_args(CyContext *ctx, size_t n) {
	CyValue popped[n];
	for (int i = 0; i < n; i++) {
		popped[i] = *pop_arg(ctx);
	}
	CyValueList *reversed = empty_cy_value_list();
	for (int i = n - 1; i >= 0; i--) {
		push_cy_value(reversed, &popped[i]);
	}
	return *reversed;
}

void cy_error(CyContext *ctx, wchar_t *message) {
	ctx->error = wcscat(ctx->error, message);
}