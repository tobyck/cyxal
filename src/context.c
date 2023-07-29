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

CyValueList *push_empty_stack(StackList *stacks) {
	stacks->stacks = realloc(stacks->stacks, (stacks->size + 1) * sizeof(CyValueList));
	stacks->stacks[stacks->size++] = empty_cy_value_list();
	return last_stack(stacks);
}

CyValueList *last_stack(StackList *stacks) {
	return stacks->stacks[stacks->size - 1];
}

void free_stack_list(StackList *list) {
	// free each stack
	for (int i = 0; i < list->size; ++i) {
		free_cy_value_list(list->stacks[i]);
	}

	// free the pointer to the array of stacks
	free(list->stacks);

	// free the struct
	free(list);
}

void pop_and_free_stack(StackList *stacks) {
	// free the stack
	free_cy_value_list(stacks->stacks[stacks->size - 1]);

	// reallocate memory of the stack list
	stacks->stacks = realloc(stacks->stacks, (--stacks->size) * sizeof(CyValueList));
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

CyValueList *pop_args(CyContext *ctx, size_t n) {
	CyValue *popped[n];
	for (int i = 0; i < n; ++i) {
		popped[i] = pop_arg(ctx);
	}

	CyValueList *reversed = empty_cy_value_list();
	for (int i = n - 1; i >= 0; i--) {
		push_cy_value(reversed, popped[i]);
	}

	return reversed;
}

void cy_error(CyContext *ctx, wchar_t *message) {
	ctx->error = wcscat(ctx->error, message);
}