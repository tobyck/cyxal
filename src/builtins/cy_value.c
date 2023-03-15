#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include "cy_value.h"
#include "../helpers.h"

// get a string from a CyType
wchar_t *stringify_cy_type(CyType type) {
    switch (type) {
        case NumberType: return L"Num";
        case StringType: return L"Str";
        case ListType: return L"List";
        case FunctionType: return L"Func";
        case NullType: return L"Null";
    }
}

// creates a new CyValue with just the provided type from the enum in cy_value.h
CyValue *cy_value_new_empty(CyType type) {
    CyValue *value = malloc(sizeof(CyValue)); // allocate just enough memory
    value->type = type; // set the type and nothing else
    return value;
}

// create a new CyValue with NumberType given a string representation of a number
CyValue *cy_value_new_num(wchar_t *num_as_str) {
    CyValue *value = cy_value_new_empty(NumberType);
    mpq_init(value->number);

    if (num_as_str == NULL) return value;

    mpz_t denominator; // arbitrary-precision integer
    mpz_init(denominator); // initialise
    mpz_set_ui(denominator, 1); // set to 1
    if (contains(num_as_str, DEC_PLACE)) { // if there's a decimal point in the number
        for (int i = 0; i < wcslen(num_as_str); i++) { // for each number in the range [0, strlen(num_as_str))
            if (num_as_str[wcslen(num_as_str) - 1 - i] == DEC_PLACE) { // if we've found the decimal point
                mpz_set_ui(denominator, pow(10, i)); // set the denominator to 10 ^ i
                break;
            }
        }
    }

    wchar_t *numerator_str = malloc(0); // initialise on the heap, so it can be dynamically append to with append_str
    for (int i = 0; i < wcslen(num_as_str); i++) {
        if (num_as_str[i] != DEC_PLACE) { // if the char is not a decimal place
            append_str(&numerator_str, chr_to_str(num_as_str[i])); // create string from char and append to numerator
        }
    }

    // if numerator is still empty i.e. there was no decimal place, set the numerator to num_as_str;
    if (wcslen(numerator_str) == 0) {
        numerator_str = num_as_str;
    }

    wchar_t *denominator_str = str_to_wcs(mpz_get_str(NULL, 10, denominator)); // create a string of the denominator

    // create a full string representation of the fraction to generate a mpq_t from
    wchar_t *fraction_str = malloc((wcslen(numerator_str) + wcslen(denominator_str) + 2) * sizeof(wchar_t));
    wcscat(fraction_str, numerator_str);
    wcscat(fraction_str, L"/");
    wcscat(fraction_str, denominator_str);

    mpq_set_str(value->number, wcs_to_str(fraction_str), 10); // create multi-precision rational
    mpq_canonicalize(value->number); // simplify

    // free memory
    free(numerator_str);
    free(denominator_str);

    return value;
}

// functions to create new CyValues from a C type

#define init_func(type, arg, arg_type) \
    CyValue *cy_value_new_##arg(arg_type arg) { \
        CyValue *value = cy_value_new_empty(type); \
        value->other = arg; \
        return value; \
    }

init_func(StringType, str, wchar_t *);
init_func(FunctionType, func, wchar_t *);
init_func(ListType, list, CyValueList *);

// functions to check if a CyValue has a certain type

#define type_check_func(short_type, type_from_enum) \
    bool cy_value_is_##short_type(CyValue value) { \
        return value.type == type_from_enum ? true : false; \
    }

type_check_func(num, NumberType);
type_check_func(str, StringType);
type_check_func(list, ListType);
type_check_func(func, FunctionType);

// returns a string representation of a cyxal value
wchar_t *stringify_cy_value(CyValue value) {
    switch (value.type) {
        case NumberType:
            return str_to_wcs(mpq_get_str(NULL, 10, value.number)); // use a builtin gmp function to stringify into base 10
        case StringType: {
            return value.other;
        }
        case ListType: {
            CyValueList *list = (CyValueList *)value.other; // variable for the list case to a (CyValueList *) from (void *)
            wchar_t *str = malloc(0);
            append_str(&str, L"[ ");
            for (int i = 0; i < list->size; i++) {
                wchar_t *item;
                bool should_free = false;
                if (cy_value_is_str(list->values[i])) {
                    item = malloc(0);
                    append_str(&item, L"\"");
                    append_str(&item, list->values[i].other);
                    append_str(&item, L"\"");
                    should_free = true;
                } else {
                    item = stringify_cy_value(list->values[i]); // stringify the item to append
                }
                append_str(&str, item);
                append_str(&str, L", ");
                if (should_free) {
                    free(item);
                }
            }
            wcscpy(str + wcslen(str) - 2, L" ]"); // replace the last ", " with " ]"
            return str;
        }
        case FunctionType: {
            size_t max_size = wcslen(value.other) + 12;
            wchar_t *str = malloc(max_size * sizeof(wchar_t));
            swprintf(str, max_size, L"<lambda \"%ls\">", (wchar_t *)value.other);
            return str;
        }
        case NullType:
            return L"<null>";
    }
}

// creates an empty CyValueList; memory isn't allocated although pointers are initialised
CyValueList *empty_cy_value_list() {
    CyValueList *list = malloc(sizeof(CyValueList));
    list->values = calloc(0, 0);
    list->size = 0;
    return list;
}

// appends one cyxal value to a cyxal list
void push_cy_value(CyValue *list, CyValue value) {
    CyValueList *as_list = list->other; // get a pointer to the actual list not CyValue
    as_list->values = realloc(as_list->values, (as_list->size + 1) * sizeof(value)); // realloc memory for the new item
    as_list->values[as_list->size++] = value; // set the last slot of the array to the new item and increment size afterwards
}