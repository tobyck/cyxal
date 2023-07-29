#include <stdarg.h>

#include "../src/builtins/elements.h"

typedef struct {
	CyValue **args;
	CyValue **output;
} TestCase;

typedef struct {
	CyElement element;
	size_t num_of_tests;
	TestCase *tests;
} ElementTest;

ElementTest new_element_test(wchar_t *symbol, size_t num_of_tests, ...) {
	CyElementList *elements = get_elements();
	CyElement element = get_element(elements, symbol);
	free_cy_element_list(elements);

	va_list args;
	va_start(args, num_of_tests);

	TestCase test_cases[num_of_tests];
	for (int i = 0; i < num_of_tests; ++i) {
		test_cases[i] = va_arg(args, TestCase);
	}

	va_end(args);

	return (ElementTest){element, num_of_tests, test_cases};
}

void test_elements(void) {
	ElementTest tests[] = {
		new_element_test(
			L"+",
			1,
			(TestCase){
				(CyValue *[]){cy_value_new_num(L"2"), cy_value_new_num(L"3")},
				(CyValue *[]){cy_value_new_num(L"5")}
			}
		)
	};

	int total = 0;
	int passed = 0;

	// for each element
	for (int i = 0; i < sizeof(tests) / sizeof(ElementTest); ++i) {
		ElementTest test = tests[i];

		// for each test case of the element
		for (int j = 0; j < test.num_of_tests; ++j) {
			TestCase test_case = test.tests[j];

			// make a new context to run the test case
			CyContext *ctx = new_cy_context(empty_cy_value_list());

			// create a dynamic array from the arguments (just for printing later)
			CyValueList *args_dynamic = empty_cy_value_list();

			// reference to the last stack
			CyValueList *_last_stack = last_stack(ctx->stacks);

			// for each argument
			for (int k = 0; k < test.element.arity_in; ++k) {
				push_cy_value(args_dynamic, clone_cy_value(test_case.args[k]));
				push_cy_value(_last_stack, clone_cy_value(test_case.args[k]));
				free_cy_value(test_case.args[k]);
			}

			// run the element
			test.element.func(ctx);

			// assume that the test passed until proven otherwise
			passed++;

			for (int k = 0; k < _last_stack->size; ++k) {
				CyValue *stack_value = _last_stack->values[k];
				CyValue *output_value = test_case.output[k];

				// if the returned value is not equal to the expected value or the element returned the wrong number of values
				if (!cy_value_eq(*stack_value, *output_value)
					|| _last_stack->size != test.element.arity_out) {
					passed--; // decrement the total passed

					printf("\n Element: %ls", test.element.symbol);
					printf("\n    Args: %ls", stringify_cy_value_list(args_dynamic));
					printf("\n     Got: %ls", stringify_cy_value_list(_last_stack));

					CyValueList *expected = empty_cy_value_list();
					for (int l = 0; l < test.element.arity_out; ++l) {
						push_cy_value(expected, clone_cy_value(test_case.output[l]));
					}

					printf("\nExpected: %ls\n", stringify_cy_value_list(expected));

					free_cy_value_list(expected);

					break;
				}
			}

			for (int k = 0; k < test.element.arity_out; ++k) {
				free_cy_value(test_case.output[k]);
			}

			free_cy_value_list(args_dynamic);

			free_cy_context(ctx);

			total++;
		}
	}

	printf(
		"Elements: %d / %d tests passed           %s%d%%\x1B[0m\n",
		passed,
		total,
		passed == total ? "\x1B[32m" : " \x1B[31m", (int)((double)passed / total * 100)
	);
}