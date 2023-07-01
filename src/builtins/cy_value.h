#ifndef CYXAL_CY_VALUE_H
#define CYXAL_CY_VALUE_H

#include <stdbool.h>
#include <gmp.h>

// enum for the type of CyValue

typedef enum {
	NumberType,
	StringType,
	ListType,
	FunctionType
} CyType;

extern wchar_t *stringify_cy_type(CyType type);

typedef struct {
	CyType type;
	mpq_t number;
	void *other;
	bool can_free;
} CyValue;

// functions to create new instances of CyValues

#define init_func_dec(type) CyValue *cy_value_new_##type // macro for CyValue initialisation function declaration

extern init_func_dec(empty)(CyType type);
extern init_func_dec(num)(wchar_t *num_as_str);
extern init_func_dec(str)(wchar_t *str);
extern init_func_dec(func)(wchar_t *src);

extern CyValue *freeable(CyValue *value);

extern void free_cy_value(CyValue *value);

// functions to check if a CyValue is a certain type

extern bool cy_value_is_num(CyValue value);
extern bool cy_value_is_str(CyValue value);
extern bool cy_value_is_list(CyValue value);
extern bool cy_value_is_func(CyValue value);

// function to stringify a CyValue to be printed

extern wchar_t *stringify_cy_value(CyValue *value);

// struct and functions for a lists of CyValues

typedef struct {
	CyValue **values;
	size_t size;
} CyValueList;

extern CyValueList *empty_cy_value_list(void);

extern void free_cy_value_list(CyValueList *list);
extern init_func_dec(list)(CyValueList *list);
extern void push_cy_value(CyValueList *list, CyValue *value);
extern CyValue *pop_cy_value(CyValueList *list);
extern bool cy_value_eq(CyValue lhs, CyValue rhs);

#endif // CYXAL_CY_VALUE_H