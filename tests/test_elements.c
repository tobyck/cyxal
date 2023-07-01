/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include "../src/context.h"
#include "../src/helpers.h"
#include "../src/builtins/cy_value.h"
#include "../src/builtins/elements.h"

typedef struct {
	CyValue **args;
	CyValue **output;
} TestCase;

typedef struct {
	CyElement element;
	size_t test_amt;
	TestCase *tests;
} ElementTest;

// stringify an element test
wchar_t *stringify_element_test(ElementTest test) {
	wchar_t *ret = malloc(0);

	append_str(&ret, L"Element ");
	append_str(&ret, test.element.symbol);
	append_str(&ret, L":\n");
	append_str(&ret, L"  Tests:\n");

	for (int i = 0; i < test.test_amt; ++i) {
		append_str(&ret, L"  ");
		for (int j = 0; j < test.element.arity_in; ++j) {
			append_str(&ret, stringify_cy_value(test.tests[i].args[j]));
		}
		append_str(&ret, L" -> ");
		for (int j = 0; j < test.element.arity_out; ++j) {
			append_str(&ret, stringify_cy_value(test.tests[i].output[j]));
		}
		append_str(&ret, L"\n");
	}

	return ret;
}

ElementTest new_element_test(wchar_t *symbol, size_t test_amt, ...) {
	CyElement element = get_element(get_elements(), symbol);

	va_list args;
	va_start(args, test_amt);
	TestCase test_cases[test_amt];
	for (int i = 0; i < test_amt; i++) {
		test_cases[i] = va_arg(args, TestCase);
	}
	va_end(args);

	ElementTest ret = {element, test_amt, test_cases};

	return ret;
}

int run_element_test(int *total, ElementTest test) {
	printf("%ls\n", stringify_element_test(test));
	return 0;
}

void test_elements(void) {
	int total_tests = 0;
	int passed_amt = 0;

	passed_amt += run_element_test(
		&total_tests,
		new_element_test(
			L"+",
			1,
			(TestCase){
				(CyValue *[]){cy_value_new_num(L"2"), cy_value_new_num(L"3")},
				(CyValue *[]){cy_value_new_num(L"5")}
			}
			*/
/*(TestCase){
				(CyValue *[]){cy_value_new_num(L"1.2"), cy_value_new_num(L".3")},
				(CyValue *[]){cy_value_new_num(L"1.5")}
			},
			(TestCase){
				(CyValue *[]){cy_value_new_str(L"abc"), cy_value_new_str(L"ABC")},
				(CyValue *[]){cy_value_new_str(L"abcABC")}
			},
			(TestCase){
				(CyValue *[]){cy_value_new_str(L"abc"), cy_value_new_num(L"123")},
				(CyValue *[]){cy_value_new_str(L"abc123")}
			}*//*

		)
	);

	*/
/*passed_amt += run_element_test(
		&total_tests,
		new_element_test(
			L"½",
			2,
			new_test_case(
				(CyValue *[]){cy_value_new_num(L"12")},
				(CyValue *[]){cy_value_new_num(L"6")}
			),
			new_test_case(
				(CyValue *[]){cy_value_new_num(L"-1.5")},
				(CyValue *[]){cy_value_new_num(L"-0.75")}
			)
		)
	);

	passed_amt += run_element_test(
		&total_tests,
		new_element_test(
			L"ka",
			1,
			new_test_case(
				(CyValue *[]){},
				(CyValue *[]){cy_value_new_str(L"abcdefghijklmnopqrstuvwxyz")}
			)
		)
	);

	passed_amt += run_element_test(
		&total_tests,
		new_element_test(
			L",",
			1,
			new_test_case(
				(CyValue *[]){cy_value_new_num(L"123")},
				(CyValue *[]){}
			)
		)
	);*//*


	printf(
		"%sElements: %d / %d tests passed           %s%d%%\x1B[0m\n",
		passed_amt == total_tests ? "" : "\n",
		passed_amt,
		total_tests,
		passed_amt == total_tests ? "\x1B[32m" : "\x1B[31m", (int)((double)passed_amt / total_tests * 100)
	);
}*/
