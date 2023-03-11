#ifndef CYXAL_CONTEXT_H
#define CYXAL_CONTEXT_H

#include "builtins/cy_value.h"

typedef struct {
    CyValueList *args;
    char *flags;
    char *output;
    char *error;
} CyContext;

extern CyContext *new_cy_context(CyValueList *args, char *flags);
// todo: change to allow formatting string directly in them - a cy_errorf sort-of thing
extern void cy_error(CyContext *ctx, char *message);

#endif // CYXAL_CONTEXT_H
