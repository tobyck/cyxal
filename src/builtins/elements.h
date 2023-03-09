#ifndef CYXAL_ELEMENTS_H
#define CYXAL_ELEMENTS_H

#include "cy_value.h"

#define nilad(name) CyValue name(void)
#define monad(name) CyValue name(CyValue *lhs)
#define dyad(name) CyValue name(CyValue *lhs, CyValue *rhs)

extern dyad(add);
extern dyad(subtract);
extern monad(halve);

#endif // CYXAL_ELEMENTS_H
