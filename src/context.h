#ifndef CYXAL_CONTEXT_H
#define CYXAL_CONTEXT_H

#include "builtins/cy_value.h"

typedef struct {
    CyValueList *args;
    CyValueList *stack;
    wchar_t *output;
    wchar_t *error;
} CyContext;

extern CyContext *new_cy_context(CyValueList *args);
// todo: change to allow formatting string directly in them - a cy_errorf sort-of thing
extern void cy_error(CyContext *ctx, wchar_t *message);

#endif // CYXAL_CONTEXT_H
