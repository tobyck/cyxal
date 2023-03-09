#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cy_value.h"

// creates a new CyValue with just the provided type from the enum in cy_value.h
CyValue *new_cy_value(CyType type) {
    CyValue *value = malloc(sizeof(CyValue));
    value->type = type;
    return value;
}

// sets the actual value of a CyValue instance to a mpq_t given a string representing a number in base 10
void cy_value_set_num(CyValue *value, char *str) {
    mpq_init(value->number);
    mpq_set_str(value->number, str, 10);
}

// sets the void *other property of a CyValue to the specified string
void cy_value_set_str(CyValue *value, char *str) {
    value->other = str;
}

// free memory for a CyValue
void free_cy_value(CyValue *value) {
    free(value->other);
    free(value);
}

// creates an empty CyValueList; memory isn't allocated although pointers are initialised
CyValueList *new_cy_value_array() {
    CyValueList *list = malloc(sizeof(CyValueList));
    list->values = calloc(0, 0);
    list->size = 0;
    return list;
}

// appends one cyxal value to a cyxal list
void push_cy_value(CyValueList *list, CyValue value) {
    list->values = realloc(list->values, (list->size + 1) * sizeof(value));
    list->values[list->size++] = value;
}

// returns a string representation of a cyxal value
char *stringify_cy_value(CyValue value) {
    switch (value.type) {
        case NumberType:
            return mpq_get_str(NULL, 10, value.number); // use a builtin gmp function to stringify into base 10
        case StringType: {
            char *str = malloc(strlen(value.other) + 2); // allocate enough memory to allow for the string plus quotes
            strcpy(str, "\"");
            strcat(str, value.other);
            strcat(str, "\"");
            return str;
        }
        case FunctionType: {
            char *str = malloc(strlen(value.other) + 9);
            sprintf(str, "<lambda \"%s\">", (char *)value.other);
            return str;
        }
        case ListType: {
            CyValueList *list = (CyValueList *)value.other; // variable for the list case to a (CyValueList *) from (void *)
            char *str = malloc(2);
            strcpy(str, "[ ");
            for (int i = 0; i < list->size; i++) {
                char *item = stringify_cy_value(list->values[i]); // stringify the item to append
                str = realloc(str, strlen(str) + strlen(item) + 2); // reallocate memory for that, the comma and space
                strcat(str, item);
                strcat(str, ", ");
            }
            strcpy(str + strlen(str) - 2, " ]"); // replace the last ", " with " ]"
            return str;
        }
    }
}

// free memory for a CyValueList
void free_cy_value_list(CyValueList *list) {
    free(list->values);
    free(list);
}