#include <locale.h>
#include "../tests/run_tests.c"

#include "context.h"
#include "builtins/cy_value.h"
#include "builtins/elements.h"

int main() {
	setlocale(LC_ALL, "");

	run_tests();

	CyContext *ctx = new_cy_context(empty_cy_value_list());
	push_cy_value(last_stack(ctx), cy_value_new_num(L"2"));
	push_cy_value(last_stack(ctx), cy_value_new_num(L"3"));
	add(ctx);
	cy_print(ctx);
	printf("2 + 3 = %ls\n", ctx->output);

	return 0;
}