#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
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
dyad(add) {
    CyValue *ret;

    if (cy_value_is_num(*lhs) && cy_value_is_num(*rhs)) {
        ret = cy_value_new_num(NULL);
        mpq_add(ret->number, lhs->number, rhs->number);
    } else if (cy_value_is_str(*lhs) || cy_value_is_str(*rhs)) {
        ret = cy_value_new_str(wcscat(lhs->other, rhs->other));
    } else {
        wchar_t err_msg[65];
        swprintf(err_msg, 65, L"Invalid combination of types for element 'add': '%ls' and '%ls'", stringify_cy_type(lhs->type), stringify_cy_type(rhs->type));
        cy_error(ctx, err_msg);
    }

    return ret;
}

/*
 * Element: ½
 * Vectorising: yes
 * Overloads
 *  (Num x) -> x * 2
 * */
monad(halve) {
    CyValue *ret;

    if (cy_value_is_num(*lhs)) {
        ret = cy_value_new_num(NULL);
        mpq_div(ret->number, lhs->number, cy_value_new_num(L"2")->number);
    } else {
        wchar_t err_msg[39];
        swprintf(err_msg, 39, L"Invalid type '%ls' for element 'halve'", stringify_cy_type(lhs->type));
        cy_error(ctx, err_msg);
    }

    return ret;
}

/*
 * Element: ka
 * Vectorising: no
 * Overloads
 *  () -> "abcdefghijklmnopqrstuvwxyz"
 * */
nilad(alphabet) {
    return cy_value_new_str(L"abcdefghijklmnopqrstuvwxyz");
}

/*
 * Element: ,
 * Vectorising: no
 * Overloads:
 *  (Any x) -> print(x)
 * */
monad(cy_print) {
    append_str(&ctx->output, stringify_cy_value(*lhs));
    return cy_value_new_empty(NullType); // return nothing
}

// function to create an empty CyElements list (you've probably seen this code at least a few times before around the place)
CyElementList *empty_cy_element_list(void) {
    CyElementList *list = malloc(sizeof(CyElementList));
    list->elements = malloc(0);
    list->size = 0;
    return list;
}

// appends an element to a list of elements
void add_element(CyElementList *list, CyElement element) {
    list->elements = realloc(list->elements, (list->size + 1) * sizeof(CyElement));
    list->elements[list->size++] = element;
}

// checks if an element is in a list of elements (used in the lexer)
bool has_element(CyElementList *list, wchar_t *symbol) {
    for (int i = 0; i < list->size; i++) {
        if (wcscmp(list->elements[i].symbol, symbol) == 0) {
            return true;
        }
    }
    return false;
}

// given a cyxal element (e.g. "+") return the corresponding function
CyElementFunc func_for_element(CyElementList *list, wchar_t *symbol) {
    for (int i = 0; i < list->size; i++) {
        if (wcscmp(list->elements[i].symbol, symbol) == 0) {
            return list->elements[i].func;
        }
    }
    return (CyElementFunc){};
}

// gets an array of all the symbols for the elements
wchar_t **elements_symbols(CyElementList *list) {
    wchar_t **symbols = malloc(list->size * sizeof(wchar_t));
    for (int i = 0; i < list->size; i++) {
        symbols[i] = list->elements[i].symbol;
    }
    return symbols;
}

CyElementList *get_elements(void) {
    CyElementList *elements = empty_cy_element_list();

    add_element(elements, (CyElement){ L"+", .func.dyad = add });
    add_element(elements, (CyElement){ L"½", .func.monad = halve });
    add_element(elements, (CyElement){ L"ka", .func.nilad = alphabet });
    add_element(elements, (CyElement){ L",", .func.monad = cy_print });

    return elements;
}