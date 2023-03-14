#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "context.h"

CyContext *new_cy_context(CyValueList *args, wchar_t *flags) {
    CyContext *ctx = malloc(sizeof(CyContext));
    ctx->args = args;
    ctx->flags = flags;
    ctx->output = malloc(0);
    ctx->error = calloc(0, 0);
    return ctx;
}

void cy_error(CyContext *ctx, wchar_t *message) {
    ctx->error = wcscat(ctx->error, message);
}