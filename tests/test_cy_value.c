#include <stdio.h>
#include "../src/builtins/cy_value.h"

void test_cy_value(void) {
	CyValue *list1 = cy_value_new_list(empty_cy_value_list());
	push_cy_value(list1->other, cy_value_new_str(L"abc"));

	CyValue *list2 = cy_value_new_list(empty_cy_value_list());
	push_cy_value(list2->other, list1);
	push_cy_value(list2->other, cy_value_new_num(L"3.2"));

	CyValue *list3 = cy_value_new_list(empty_cy_value_list());
	push_cy_value(list3->other, cy_value_new_num(L".625"));
	push_cy_value(list3->other, cy_value_new_str(L"123"));
	push_cy_value(list3->other, list2);

	printf("%ls\n", stringify_cy_value(*list3));

	free_cy_value(list3);
}