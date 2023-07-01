#ifndef CYXAL_CONTEXT_H
#define CYXAL_CONTEXT_H

#include "builtins/cy_value.h"

typedef struct {
	CyValueList **stacks;
	size_t size;
} StackList;

typedef struct {
	CyValueList *args; // fixed array of program arguments
	int arg_index; // index of arg to pop next (if stack is empty)
	StackList *stacks;
	wchar_t *output;
	wchar_t *error;
} CyContext;

extern CyContext *new_cy_context(CyValueList *args);
extern void free_cy_context(CyContext *ctx);
// todo: change to allow formatting string directly in them - a cy_errorf sort-of thing
extern void cy_error(CyContext *ctx, wchar_t *message);

extern StackList *empty_stack_list(void);
extern void free_stack_list(StackList *list);
extern void push_empty_stack(StackList *ctx);
extern CyValueList *last_stack(StackList *stacks);
extern CyValueList *pop_stack(StackList *stacks);
extern CyValue *pop_arg(CyContext *ctx);
extern CyValueList pop_args(CyContext *ctx, size_t n);

#endif // CYXAL_CONTEXT_H
