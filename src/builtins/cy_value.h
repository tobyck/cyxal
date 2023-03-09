#ifndef CYXAL_CY_VALUE_H
#define CYXAL_CY_VALUE_H

#include "../../gmp/gmp.h"

// enum for the type of cyxal value

typedef enum {
    NumberType,
    StringType,
    ListType,
    FunctionType
} CyType;

/*
 * the cyxal value struct is able to store any type available  in cyxal - numbers
 * are stored in gmp's arbitrary precision rational type, strings are stored in
 * the void pointer field (cast to char pointer first), functions are stored as cyxal
 * source code which is evaluated as needed, and lists are also stored where strings are,
 * but as a pointer to a CyValueList.
 * */

typedef struct {
    CyType type;
    mpq_t number;
    void *other;
} CyValue;

extern CyValue *new_cy_value(CyType type);
extern void cy_value_set_num(CyValue *value, char *str);
extern void cy_value_set_str(CyValue *value, char *str);
extern char *stringify_cy_value(CyValue value);
extern void free_cy_value(CyValue *value);

typedef struct {
    CyValue *values;
    size_t size;
} CyValueList;

extern CyValueList *new_cy_value_array();
extern void push_cy_value(CyValueList *list, CyValue value);
extern void free_cy_value_list(CyValueList *list);

#endif // CYXAL_CY_VALUE_H
