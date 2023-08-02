#include <stdbool.h>

#include "test_cy_value.h"
#include "test_lexer.h"
#include "test_elements.h"

int main(void) {
	bool passed_cy_value_tests = test_cy_value();
	bool passed_lexer_tests = test_lexer();
	bool passed_elements_tests = test_elements();

	return passed_cy_value_tests && passed_lexer_tests && passed_elements_tests ? 0 : 1;
}