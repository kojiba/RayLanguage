#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <string.h>
#include "../RayFoundation.h"

typedef struct Function {
    size_t pointer;
    byte *body;
    uint64_t size;
} Function;

Function *funcToByte(pointer begin, pointer end);

#endif /*__FUNCTION_H__*/