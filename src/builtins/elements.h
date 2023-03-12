#ifndef CYXAL_ELEMENTS_H
#define CYXAL_ELEMENTS_H

#include "cy_value.h"
#include "../context.h"

// macros to create function headers for different arities

#define nilad(name) CyValue *name(CyContext *ctx)
#define monad(name) CyValue *name(CyContext *ctx, CyValue *lhs)
#define dyad(name) CyValue *name(CyContext *ctx, CyValue *lhs, CyValue *rhs)

// declerations for elements (to be defined in elements.c)

extern dyad(add);
extern monad(halve);
extern nilad(alphabet);
extern monad(print);

// types for storing element

typedef union {
    CyValue *(*nilad)(CyContext *);
    CyValue *(*monad)(CyContext *, CyValue *);
    CyValue *(*dyad)(CyContext *, CyValue *, CyValue *);
} CyElementFunc;

typedef struct {
    char *symbol; // the char(s) used in Cyxal to use the element e.g. `W` or `kA`
    CyElementFunc func; // pointer to a function for the element
} CyElement;

// struct to store an array of CyElements and their size

typedef struct {
    CyElement *elements; // pointer to the first element
    size_t size;
} CyElementList;

// functions for initialising and accessing CyElementLists

extern CyElementList *empty_cy_element_list(void);
extern void add_element(CyElementList *list, CyElement element);
extern CyElementList *get_elements(void);
extern bool has_element(CyElementList *list, char *symbol);
extern CyElementFunc func_for_element(CyElementList *list, char *symbol);
extern char **elements_symbols(CyElementList *list);

#endif // CYXAL_ELEMENTS_H
