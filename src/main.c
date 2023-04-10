#include <locale.h>
#include "../tests/run_tests.c"

#include "context.h"
#include "builtins/cy_value.h"
#include "builtins/elements.h"

int main() {
	setlocale(LC_ALL, "");

	//run_tests();

	CyContext *ctx = new_cy_context(empty_cy_value_list());
	free_cy_context(ctx);

	return 0;
}