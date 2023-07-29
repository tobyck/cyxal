#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include "cy_value.h"
#include "../helpers.h"

// get a string from a CyType
wchar_t *stringify_cy_type(CyType type) {
	switch (type) {
		case NumberType:
			return L"Num";
		case StringType:
			return L"Str";
		case ListType:
			return L"List";
		case FunctionType:
			return L"Func";
	}
}

// creates a new CyValue with just the provided type from the enum in cy_value.h
CyValue *cy_value_new_empty(CyType type) {
	CyValue *value = malloc(sizeof(CyValue)); // allocate just enough memory
	value->type = type;
	return value;
}

// create a new CyValue with NumberType given a string representation of a number
CyValue *cy_value_new_num(wchar_t *num_as_str) {
	CyValue *value = cy_value_new_empty(NumberType);

	mpq_init(value->number);

	if (num_as_str == NULL) return value; // if no string was provided, return the empty CyValue

	mpz_t denominator; // arbitrary-precision integer to store the denominator
	mpz_init(denominator); // initialise
	mpz_set_ui(denominator, 1); // set to 1
	if (contains(num_as_str, DEC_PLACE)) { // if there's a decimal point in the number
		for (int i = 0; i < wcslen(num_as_str); ++i) { // for each number in the range [0, strlen(num_as_str))
			if (num_as_str[wcslen(num_as_str) - 1 - i] == DEC_PLACE) { // if we've found the decimal point
				mpz_set_ui(denominator, pow(10, i)); // set the denominator to 10 ^ i
				break;
			}
		}
	}

	wchar_t *numerator_str = malloc(0); // initialise on the heap, so it can be dynamically appended to with append_str
	for (int i = 0; i < wcslen(num_as_str); ++i) {
		if (num_as_str[i] != DEC_PLACE) { // if the char is not a decimal place
			wchar_t *chr_as_str = chr_to_wcs(num_as_str[i]); // store it as a wide char string
			append_str_and_free(&numerator_str, chr_as_str); // then append to numerator_str
		}
	}

	// if numerator is still empty i.e. there was no decimal place, set the numerator to num_as_str;
	if (wcslen(numerator_str) == 0) {
		numerator_str = num_as_str;
	}

	char *normal_str = mpz_get_str(
		NULL, 10, denominator
	); // stringify the denominator (mpz_t) and store in a normal string
	wchar_t *denominator_str = str_to_wcs(normal_str); // then convert to wide char string
	free(normal_str);
	mpz_clear(denominator);

	// create a full string representation of the fraction to generate a mpq_t from
	wchar_t *fraction_str = malloc((wcslen(numerator_str) + wcslen(denominator_str) + 2) * sizeof(wchar_t));
	wcscat(fraction_str, numerator_str);
	wcscat(fraction_str, L"/");
	wcscat(fraction_str, denominator_str);

	free(numerator_str);
	free(denominator_str);

	char *fraction_as_normal_str = wcs_to_str(fraction_str);
	free(fraction_str);
	mpq_set_str(value->number, fraction_as_normal_str, 10);
	free(fraction_as_normal_str);

	return value;
}

CyValue *cy_value_new_str(wchar_t *str) {
	CyValue *value = cy_value_new_empty(StringType);
	value->other = malloc((wcslen(str) + 1) * sizeof(wchar_t));
	wcscpy(value->other, str);
	return value;
}

// macro to create a function that makes a CyValue with a certain type and `other` field
#define init_func(type, arg, arg_type) \
    CyValue *cy_value_new_##arg(arg_type arg) {    \
        CyValue *value = cy_value_new_empty(type); \
        value->other = arg;                        \
        return value;                              \
    }

init_func(FunctionType, func, wchar_t *);

init_func(ListType, list, CyValueList *);

// functions to check if a CyValue has a certain type

#define type_check_func(short_type, type_from_enum) \
    bool cy_value_is_##short_type(CyValue value) {          \
        return value.type == type_from_enum ? true : false; \
    }

type_check_func(num, NumberType);

type_check_func(str, StringType);

type_check_func(list, ListType);

type_check_func(func, FunctionType);

// free all memory associated with a CyValue
void free_cy_value(CyValue *value) {
	if (value->type == NumberType) {
		mpq_clear(value->number);
	} else if (value->type == StringType) {
		free(value->other);
	} else if (value->type == ListType) {
		free_cy_value_list(value->other);
	}

	free(value);
}

// returns a string representation of a CyValue
wchar_t *stringify_cy_value(CyValue *value) {
	wchar_t *ret;

	if (value->type == NumberType) {
		char *normal_str = mpq_get_str(NULL, 10, value->number); // use a builtin gmp function to stringify into base 10
		ret = str_to_wcs(normal_str);
		free(normal_str);
	} else if (value->type == StringType) {
		// set ret to value->other as a string and make sure it's on the heap
		ret = wcsdup(value->other);
	} else if (value->type == ListType) {
		ret = stringify_cy_value_list(value->other);
	} else if (value->type == FunctionType) {
		ret = wcsdup(L"<function>");
	} else {
		fprintf(stderr, "stringify_cy_value: unknown type %d\n", value->type);
		return NULL;
	}

	return ret;
}

// creates an empty CyValueList; memory isn't allocated although pointers are initialised
CyValueList *empty_cy_value_list(void) {
	CyValueList *list = malloc(sizeof(CyValueList));
	list->values = malloc(0);
	list->size = 0;
	return list;
}

CyValueList *clone_cy_value_list(CyValueList *list) {
	CyValueList *new_list = empty_cy_value_list();

	// TODO: more efficient way to do this?
	for (int i = 0; i < list->size; ++i) {
		push_cy_value(new_list, clone_cy_value(list->values[i]));
	}

	return new_list;
}

wchar_t *stringify_cy_value_list(CyValueList *list) {
	wchar_t *ret = malloc(0);

	// if the list is empty, return "[]"
	if (list->size == 0) {
		return L"[]";
	}

	append_str(&ret, L"[ "); // append the opening bracket

	for (int i = 0; i < list->size; ++i) { // for each item
		wchar_t *item; // string to store the item to append

		if (cy_value_is_str(*list->values[i])) { // if the item is a string
			// append the string with quotes around it
			item = malloc(0);
			append_str(&item, L"\"");
			append_str(&item, list->values[i]->other);
			append_str(&item, L"\"");
		} else {
			// otherwise stringify the item normally
			item = stringify_cy_value(list->values[i]);
		}
		// add item to final string and append comma
		append_str_and_free(&ret, item);
		append_str(&ret, L", ");
	}

	wcscpy(ret + wcslen(ret) - 2, L" ]"); // replace the last ", " with " ]"

	return ret;
}

// free all memory associated with a CyValueList
void free_cy_value_list(CyValueList *list) {
	// free all values in the list
	for (int i = 0; i < list->size; ++i) {
		free_cy_value(list->values[i]);
	}

	free(list->values);

	// free the struct itself
	free(list);
}

// appends one cyxal value to a CyValueList
void push_cy_value(CyValueList *list, CyValue *value) {
	list->values = realloc(list->values, (list->size + 1) * sizeof(CyValue)); // realloc memory for the new item
	list->values[list->size++] = value; // set the last slot of the array to the new item and increment size afterwards
}

// remove the last item of a CyValueList list and return it
CyValue *pop_cy_value(CyValueList *list) {
	// extract the last value from the list (this *seems* to not be freed by the next line but I'm not sure why tbh)
	CyValue *last_value = list->values[list->size - 1];
	// deallocate the last slot of the array and decrement size afterwards
	list->values = realloc(list->values, (--list->size) * sizeof(CyValue));
	return last_value;
}

// checks if two CyValues are equal
bool cy_value_eq(CyValue lhs, CyValue rhs) {
	if (lhs.type == NumberType && rhs.type == NumberType) {
		return mpq_equal(lhs.number, rhs.number) ? true : false;
	} else if (lhs.type == StringType && rhs.type == StringType) {
		return wcscmp(lhs.other, rhs.other) == 0 ? true : false;
	} else if (lhs.type == ListType && rhs.type == ListType) {
		CyValueList *lhs_as_list = (CyValueList *)lhs.other;
		CyValueList *rhs_as_list = (CyValueList *)rhs.other;
		if (lhs_as_list->size == rhs_as_list->size) {
			for (int i = 0; i < lhs_as_list->size; ++i) {
				if (!cy_value_eq(*lhs_as_list->values[i], *rhs_as_list->values[i])) {
					return false;
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

CyValue *clone_cy_value(CyValue *value) {
	CyValue *clone = malloc(sizeof(CyValue));

	clone->type = value->type;

	if (value->type == NumberType) {
		mpq_init(clone->number);
		mpq_set(clone->number, value->number);
	} else if (value->type == StringType) {
		clone->other = wcsdup(value->other);
	} else if (value->type == ListType) {
		clone->other = clone_cy_value_list(value->other);
	} else if (value->type == FunctionType) {
		clone->other = value->other;
	}

	return clone;
}