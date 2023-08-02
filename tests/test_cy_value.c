#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "../src/builtins/cy_value.h"

bool test_cy_value(void) {
	CyValue *list1 = cy_value_new_list(empty_cy_value_list());
	push_cy_value(list1->other, cy_value_new_str(L"abc"));

	CyValue *list2 = cy_value_new_list(empty_cy_value_list());
	push_cy_value(list2->other, list1);
	push_cy_value(list2->other, cy_value_new_num(L"3.2"));

	CyValue *list3 = cy_value_new_list(empty_cy_value_list());
	push_cy_value(list3->other, cy_value_new_num(L".625"));
	push_cy_value(list3->other, cy_value_new_str(L"123"));
	push_cy_value(list3->other, list2);

	wchar_t *str = stringify_cy_value(list3);
	wchar_t *expected = L"[ 625/1000, \"123\", [ [ \"abc\" ], 32/10 ] ]";

	bool passed = false;

	if (wcscmp(str, expected) == 0) {
		printf("\x1B[0mCyValues stringifying properly         \x1B[32mPASSED\x1B[0m\n");
		passed = true;
	} else {
		printf("CyValues not stringifying properly     \x1B[31mFAILED\x1B[0m\n\n");
		printf("Expected: %ls\n", expected);
		printf("     Got: %ls\n\n", str);
	}

	free(str);

	free_cy_value(list3);

	return passed;
}