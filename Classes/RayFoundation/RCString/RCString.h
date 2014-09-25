#ifndef __R_C_STRING_H__
#define __R_C_STRING_H__

#include "../RayFoundation.h"
#include "../RArray/RFinderDelegate/RFinderDelegate.h"

class(RCString)
    char *baseString;
    uint64_t size;
endOfClass(RCString)

constructor(RCString));
destructor(RCString);

method(RCString *, setString, RCString), char *string);
method(RFinderDelegateFlags, compareWith, RCString), RCString *checkString);

printer(RCString);


#define makeRCString() $(NULL, c(RCString)))
#define printRString(_RCString) $(_RCString, p(RCString)) );
#define RS(CString) $(makeRCString(), m(setString, RCString)), CString);

#endif /*__R_C_STRING_H__*/