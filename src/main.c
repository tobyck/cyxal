#include <locale.h>
#include "../tests/run_tests.c"

#include "context.h"
#include "builtins/cy_value.h"
#include "builtins/elements.h"
#include "helpers.h"

int main() {
	setlocale(LC_ALL, "");

	// all of the following tests don't leak memory :)

	/*CyValue *num = cy_value_new_num(L"-.23");
	wchar_t *stringified_num = stringify_cy_value(num);
	printf("%ls\n", stringified_num);
	free(stringified_num);
	free_cy_value(num);*/

	/*CyValue *str = cy_value_new_str(L"Tēnā koe!");
	wchar_t *stringified_str = stringify_cy_value(str);
	printf("%ls\n", stringified_str);
	free(stringified_str);
	free_cy_value(str);*/

	/*CyValueList *list = empty_cy_value_list();
	push_cy_value(list, cy_value_new_num(L"4.5"));
	free_cy_value_list(list);*/

	/*StackList *stacks = empty_stack_list();
	push_empty_stack(stacks);
	push_cy_value(last_stack(stacks), cy_value_new_num(L"6"));
	push_cy_value(last_stack(stacks), cy_value_new_num(L"7"));
	free_stack_list(stacks);*/

	/*CyContext *ctx = new_cy_context(empty_cy_value_list());
	push_cy_value(last_stack(ctx->stacks), cy_value_new_num(L"2"));
	push_cy_value(last_stack(ctx->stacks), cy_value_new_num(L"3"));
	free_cy_context(ctx);*/

	// add two numbers
	/*CyContext *ctx = new_cy_context(empty_cy_value_list());
	push_cy_value(last_stack(ctx->stacks), cy_value_new_num(L"2"));
	push_cy_value(last_stack(ctx->stacks), cy_value_new_num(L"3"));
	add(ctx);
	cy_print(ctx);
	printf("%ls\n", ctx->output);
	free_cy_context(ctx);*/

	return 0;
}