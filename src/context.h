#ifndef CYXAL_CONTEXT_H
#define CYXAL_CONTEXT_H

#include "builtins/cy_value.h"

typedef struct {
    CyValueList **stacks;
    size_t size;
} StackList;

typedef struct {
    CyValueList *args;
    StackList *stacks;
    wchar_t *output;
    wchar_t *error;
} CyContext;

extern CyContext *new_cy_context(CyValueList *args);
// todo: change to allow formatting string directly in them - a cy_errorf sort-of thing
extern void cy_error(CyContext *ctx, wchar_t *message);

extern StackList *empty_stack_list(void);
extern void push_empty_stack(CyContext *ctx);
extern CyValueList *last_stack(CyContext *ctx);
extern CyValueList *pop_stack(CyContext *ctx);
extern CyValue *pop_arg(CyContext *ctx);

#endif // CYXAL_CONTEXT_H
