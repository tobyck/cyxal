#ifndef CYXAL_ELEMENTS_H
#define CYXAL_ELEMENTS_H

#include "cy_value.h"
#include "../context.h"

#define element(name) void name(CyContext *ctx)

// declarations for elements (to be defined in elements.c)

extern element(add);
extern element(vectorising_add);
extern element(halve);
extern element(vectorising_halve);
extern element(alphabet);
extern element(cy_print);

// types for storing element

typedef void (*CyElementFunc)(CyContext *);

typedef struct {
	wchar_t *symbol; // the char(s) used in Cyxal to use the element e.g. `W` or `kA`
	CyElementFunc func; // pointer to a function for the element
	size_t arity_in; // how many arguments the element takes
	size_t arity_out; // how many values the element pushes
} CyElement;

// struct to store an array of CyElements and their size

typedef struct {
	CyElement *elements; // pointer to the first element
	size_t size;
} CyElementList;

// functions for initialising and accessing CyElementLists

extern CyElementList *empty_cy_element_list(void);
extern void free_cy_element_list(CyElementList *list);
extern void add_element(CyElementList *list, CyElement element);
extern CyElementList *get_elements(void);
extern CyElement get_element(CyElementList *list, wchar_t *symbol);
extern bool has_element(CyElementList *list, wchar_t *symbol);
extern wchar_t **elements_symbols(CyElementList *list);

#endif // CYXAL_ELEMENTS_H
