#include <stdlib.h>
#include <string.h>

#include "context.h"

CyContext *new_cy_context(CyValueList *args, char *flags) {
    CyContext *ctx = malloc(sizeof(CyContext));
    ctx->args = args;
    ctx->flags = flags;
    ctx->output = malloc(0);
    ctx->error = calloc(0, 0);
    return ctx;
}

void cy_error(CyContext *ctx, char *message) {
    ctx->error = strcat(ctx->error, message);
}