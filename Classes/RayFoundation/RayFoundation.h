/**
 * RayFoundation.h
 * A ray of light in the realm of darkness.
 * Some additions to C.
 * If You don't like it, You can preprocess file, to get pure-C code.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __RAY_FOUNDATION__
#define __RAY_FOUNDATION__

#ifdef  __cplusplus
extern "C" {
#endif

// Workers
#include "RSyntax.h"
#include "RColors.h"
#include "RBasics/RBasics.h"

// Containers
#include "RContainers/RArray.h"
#include "RContainers/RBuffer.h"
#include "RContainers/RDictionary.h"
#include "RContainers/RStringDictionary.h"

// Strings
#include "RCString/RCString.h"
#include "RCString/RCString_Numbers.h"

// Memory operations
#include "RMemoryOperations/RByteOperations.h"
#include "RMemoryOperations/RSandBox.h"

// Others
#include "RClassTable/RClassTable.h"
#include "RFloatingEnum/RFloatingEnum.h"

#ifdef  __cplusplus
}
#endif

#endif /*__RAY_FOUNDATION__*/