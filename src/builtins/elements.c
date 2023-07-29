#include <stdlib.h>
#include <wchar.h>
#include "elements.h"
#include "../helpers.h"

/*
 * Element: +
 * Vectorising: yes
 * Overloads:
 *  (Num x, Num y) -> x + y
 *  (Str x, Num y) -> concat(x, Str(y))
 *  (Num x, Str y) -> concat(Str(x), y)
 *  (Str x, Str y) -> concat(x, y)
 * */

element(add) {
	CyValue *rhs = pop_arg(ctx);
	CyValue *lhs = pop_arg(ctx);

	CyValue *ret;

	if (cy_value_is_num(*lhs) && cy_value_is_num(*rhs)) {
		ret = cy_value_new_num(NULL); // empty CyValue number
		mpq_add(ret->number, lhs->number, rhs->number);
	} else if (cy_value_is_str(*lhs) || cy_value_is_str(*rhs)) {
		wchar_t *str_on_heap = malloc(0);
		append_str_and_free(&str_on_heap, stringify_cy_value(lhs));
		append_str_and_free(&str_on_heap, stringify_cy_value(rhs));
		ret = cy_value_new_str(str_on_heap);
	} else {
		wchar_t err_msg[65];
		swprintf(
			err_msg, 65, L"Invalid combination of types for element 'add': '%ls' and '%ls'",
			stringify_cy_type(lhs->type), stringify_cy_type(rhs->type)
		);
		cy_error(ctx, err_msg);
		return;
	}

	free_cy_value(lhs);
	free_cy_value(rhs);

	push_cy_value(last_stack(ctx->stacks), ret);
}

element(vectorising_add) {
	CyElement el = {NULL, add, 2, 1};
	vectorise(ctx, el);
}

/*
 * Element: ½
 * Vectorising: yes
 * Overloads
 *  (Num x) -> x * 2
 * */

element(halve) {
	CyValue *lhs = pop_arg(ctx);

	CyValue *ret;

	if (cy_value_is_num(*lhs)) {
		ret = cy_value_new_num(NULL);
		CyValue *number_2 = cy_value_new_num(L"2");
		mpq_div(ret->number, lhs->number, number_2->number);
		free_cy_value(number_2);
	} else {
		wchar_t err_msg[39];
		swprintf(err_msg, 39, L"Invalid type '%ls' for element 'halve'", stringify_cy_type(lhs->type));
		cy_error(ctx, err_msg);
		return;
	}

	free_cy_value(lhs);

	push_cy_value(last_stack(ctx->stacks), ret);
}

element(vectorising_halve) {
	CyElement el = {NULL, halve, 1, 1};
	vectorise(ctx, el);
}

/*
 * Element: ka
 * Vectorising: no
 * Overloads
 *  () -> "abcdefghijklmnopqrstuvwxyz"
 * */

element(alphabet) {
	push_cy_value(last_stack(ctx->stacks), cy_value_new_str(L"abcdefghijklmnopqrstuvwxyz"));
}

/*
 * Element: ,
 * Vectorising: no
 * Overloads:
 *  (Any x) -> print(x)
 * */

element(cy_print) {
	CyValue *value = pop_arg(ctx);
	append_str_and_free(&ctx->output, stringify_cy_value(value));
	free_cy_value(value);
}

// function to create an empty CyElements list (you've probably seen this code at least a few times before around the place)
CyElementList *empty_cy_element_list(void) {
	CyElementList *list = malloc(sizeof(CyElementList));
	list->elements = malloc(0);
	list->size = 0;
	return list;
}

void free_cy_element_list(CyElementList *list) {
	free(list->elements);
	free(list);
}

// appends an element to a list of elements
void add_element(CyElementList *list, CyElement element) {
	list->elements = realloc(list->elements, (list->size + 1) * sizeof(CyElement));
	list->elements[list->size++] = element;
}

// checks if an element is in a list of elements (used in the lexer)
bool has_element(CyElementList *list, wchar_t *symbol) {
	for (int i = 0; i < list->size; ++i) {
		if (wcscmp(list->elements[i].symbol, symbol) == 0) {
			return true;
		}
	}
	return false;
}

// given a cyxal element (e.g. "+") return the corresponding function
CyElementFunc func_for_element(CyElementList *list, wchar_t *symbol) {
	for (int i = 0; i < list->size; ++i) {
		if (wcscmp(list->elements[i].symbol, symbol) == 0) {
			return list->elements[i].func;
		}
	}
	return NULL;
}

// gets an array of all the symbols for the elements
wchar_t **elements_symbols(CyElementList *list) {
	wchar_t **symbols = malloc(list->size * sizeof(wchar_t));
	for (int i = 0; i < list->size; ++i) {
		symbols[i] = list->elements[i].symbol;
	}
	return symbols;
}

CyElement get_element(CyElementList *list, wchar_t *symbol) {
	for (int i = 0; i < list->size; ++i) {
		if (wcscmp(list->elements[i].symbol, symbol) == 0) {
			return list->elements[i];
		}
	}

	return (CyElement){};
}

CyElementList *get_elements(void) {
	CyElementList *elements = empty_cy_element_list();

	add_element(elements, (CyElement){L"+", vectorising_add, 2, 1});
	add_element(elements, (CyElement){L"½", vectorising_halve, 1, 1});
	add_element(elements, (CyElement){L"ka", alphabet, 0, 1});
	add_element(elements, (CyElement){L",", cy_print, 1, 0});

	return elements;
}