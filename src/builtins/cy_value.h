#ifndef CYXAL_CY_VALUE_H
#define CYXAL_CY_VALUE_H

#include <stdbool.h>
#include "../../gmp/gmp.h"

// enum for the type of CyValue

typedef enum {
    NumberType,
    StringType,
    ListType,
    FunctionType,
    NullType // for elements which don't push anything
} CyType;

extern char *stringify_cy_type(CyType type);

/*
 * the cyxal value struct is able to store any type available in cyxal - numbers
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

// functions to create new instances of CyValues

#define init_func_dec(type) CyValue *cy_value_new_##type // macro for CyValue initialisation function declaration

extern init_func_dec(empty)(CyType type);
extern init_func_dec(num)(char *new_str);
extern init_func_dec(str)(char *str);
extern init_func_dec(func)(char *src);

// functions to check if a CyValue is a certain type

extern bool cy_value_is_num(CyValue value);
extern bool cy_value_is_str(CyValue value);
extern bool cy_value_is_list(CyValue value);
extern bool cy_value_is_func(CyValue value);

// function to stringify a CyValue to be printed

extern char *stringify_cy_value(CyValue value);

// struct and functions for a lists of CyValues

typedef struct {
    CyValue *values;
    size_t size;
} CyValueList;

extern CyValueList *empty_cy_value_list();
extern init_func_dec(list)(CyValueList *list);
extern void push_cy_value(CyValue *list, CyValue value);

#endif // CYXAL_CY_VALUE_H
