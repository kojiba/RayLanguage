#ifndef __R_BASE64_H__
#define __R_BASE64_H__

#include "../RCString.h"
#include "../../RByteOperations/RByteOperations.h"

static const char Base64Table[64];

// Bytes
method(RByteArray *, toBase64,   RByteArray)); // encodes
method(RByteArray *, fromBase64, RByteArray)); // decodes

// Strings
method(RCString *,   toBase64,   RCString));
method(RCString *,   fromBase64, RCString));

#endif /*__R_BASE64_H__*/