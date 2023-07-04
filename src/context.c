#include <stdlib.h>
#include <wchar.h>
#include "context.h"

// make a new context with the given arguments
CyContext *new_cy_context(CyValueList *args) {
	CyContext *ctx = malloc(sizeof(CyContext));
	ctx->args = args;
	ctx->arg_index = 0;
	ctx->stacks = empty_stack_list();
	push_empty_stack(ctx->stacks);
	ctx->output = malloc(0);
	ctx->error = malloc(0);
	return ctx;
}

// free a CyContext and everything in it
void free_cy_context(CyContext *ctx) {
	free_cy_value_list(ctx->args);
	free_stack_list(ctx->stacks);
	free(ctx->output);
	free(ctx->error);
	free(ctx);
}

StackList *empty_stack_list(void) {
	StackList *list = malloc(sizeof(StackList));
	list->stacks = malloc(0);
	list->size = 0;
	return list;
}

void push_empty_stack(StackList *stacks) {
	stacks->stacks = realloc(stacks->stacks, (stacks->size + 1) * sizeof(CyValueList));
	stacks->stacks[stacks->size++] = empty_cy_value_list();
}

CyValueList *last_stack(StackList *stacks) {
	return stacks->stacks[stacks->size - 1];
}

void free_stack_list(StackList *list) {
	// free each stack
	for (int i = 0; i < list->size; ++i) {
		// free the stack
		free_cy_value_list(list->stacks[i]);
	}

	free(list->stacks);

	// free the struct
	free(list);
}

CyValueList *pop_stack(StackList *stacks) {
	CyValueList *ret = last_stack(stacks);
	stacks->stacks = realloc(stacks->stacks, (--stacks->size) * sizeof(CyValueList));
	return ret;
}

CyValue *pop_arg(CyContext *ctx) {
	if (last_stack(ctx->stacks)->size > 0) {
		return pop_cy_value(last_stack(ctx->stacks));
	} else if (ctx->args->size > 0) {
		CyValue *ret = ctx->args->values[ctx->arg_index];
		ctx->arg_index++;
		ctx->arg_index %= ctx->args->size;
		return ret;
	} else {
		return cy_value_new_num(L"0");
	}
}

CyValueList pop_args(CyContext *ctx, size_t n) {
	CyValue popped[n];
	for (int i = 0; i < n; ++i) {
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