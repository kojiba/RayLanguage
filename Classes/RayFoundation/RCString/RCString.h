#ifndef __R_C_STRING_H__
#define __R_C_STRING_H__

#include "../RayFoundation.h"

typedef enum RCStringFlags{
    equals,
    not_equals
} RCStringFlags;

class(RCString)
    char *baseString;
    uint64_t size;
endOfClass(RCString)

constructor(RCString));

destructor(RCString);

byte compareCStrings(char *first, char *second);
method(void, setString, RCString), char *string);
method(byte, compareWith, RCString), RCString *checkString);


#define makeRCString() $(NULL, c(RCString)))

#endif /*__R_C_STRING_H__*/