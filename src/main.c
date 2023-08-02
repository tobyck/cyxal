#include <locale.h>
#include <stdlib.h>

#include "context.h"
#include "builtins/cy_value.h"
#include "builtins/elements.h"
#include "helpers.h"
#include "parser/lexer.h"

int main() {
	setlocale(LC_ALL, "");

	// everything done here doesn't leak memory :)

	// prints "-23/100"
	CyValue *num = cy_value_new_num(L"-.23");
	wchar_t *stringified_num = stringify_cy_value(num);
	printf("%ls\n", stringified_num);
	free(stringified_num);
	free_cy_value(num);

	// prints "Tēnā koe!"
	CyValue *str = cy_value_new_str(L"Tēnā koe!");
	wchar_t *stringified_str = stringify_cy_value(str);
	printf("%ls\n", stringified_str);
	free(stringified_str);
	free_cy_value(str);

	// prints "[ 45/100, "Abc" ]"
	CyValueList *list1 = empty_cy_value_list();
	push_cy_value(list1, cy_value_new_num(L"4.5"));
	push_cy_value(list1, cy_value_new_str(L"Abc"));
	CyValue *list_val = cy_value_new_list(list1);
	wchar_t *stringified_list = stringify_cy_value(list_val);
	printf("%ls\n", stringified_list);
	free(stringified_list);
	free_cy_value(list_val);

	// test stack list
	StackList *stacks = empty_stack_list();
	push_empty_stack(stacks);
	push_cy_value(last_stack(stacks), cy_value_new_num(L"6"));
	push_cy_value(last_stack(stacks), cy_value_new_num(L"7"));
	free_stack_list(stacks);

	// add two numbers
	CyContext *ctx1 = new_cy_context(empty_cy_value_list());
	push_cy_value(last_stack(ctx1->stacks), cy_value_new_num(L"2"));
	push_cy_value(last_stack(ctx1->stacks), cy_value_new_num(L"3"));
	vectorising_add(ctx1);
	cy_print(ctx1);
	printf("%ls\n", ctx1->output);
	free_cy_context(ctx1);

	// add a list and a number
	CyValue *list2 = cy_value_new_list(empty_cy_value_list());
	push_cy_value(list2->other, cy_value_new_num(L"2"));
	push_cy_value(list2->other, cy_value_new_num(L"3"));

	CyValue *num2 = cy_value_new_num(L"4");

	CyContext *ctx2 = new_cy_context(empty_cy_value_list());
	push_cy_value(last_stack(ctx2->stacks), list2);
	push_cy_value(last_stack(ctx2->stacks), num2);

	vectorising_add(ctx2);
	cy_print(ctx2);
	printf("%ls\n", ctx2->output);
	free_cy_context(ctx2);

	// lexer test
	CyTokenList *tokens = lex(L"2 3H+");

	for (int i = 0; i < tokens->size; ++i) {
		wchar_t *stringified_token = stringify_cy_token(tokens->tokens[i]);
		printf("%ls\n", stringified_token);
		free(stringified_token);
	}

	free_cy_token_list(tokens, true);

	return 0;
}