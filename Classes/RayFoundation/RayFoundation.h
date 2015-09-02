/**
 * RayFoundation.h
 * A ray of light in the realm of darkness.
 * Some additions to C.
 * If You don't like it, You can preprocess files, to get pure-C code.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   " (_) | | | |_) | (_| |
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
#include "RSystem.h"
#include "RSyntax.h"
#include "RColors.h"
#include "RBasics/RBasics.h"

// Containers
#include "RContainers/RArray.h"
#include "RContainers/RList.h"
#include "RContainers/RBuffer.h"
#include "RContainers/RDictionary.h"

// Strings
#include "RCString/RCString.h"
#include "RCString/RCString_Char.h"
#include "RCString/RCString_Numbers.h"
#include "RCString/RString.h"
#include "RCString/RString_Consts.h"
#include "RCString/RCString_File.h"

// Memory operations
#include "RMemoryOperations/RByteOperations.h"
#include "RMemoryOperations/RSandBox.h"
#include "RMemoryOperations/RAutoPool.h"

// Encoding
#include "REncoding/RBase64.h"

// Networking
#include "RNetwork/RSocket.h"
#include "RNetwork/RTCPHandler.h"

// Threads
#include "RThread/RThread.h"
#include "RThread/RThreadPool.h"

// Others
#include "RClassTable/RClassTable.h"
#include "Utils/Utils.h"

#define endRay() deleter(stringConstantsTable(), RDictionary);\
                 deleter(RCTSingleton, RClassTable); \
                 p(RAutoPool)(RPool); \
                 deleter(RPool, RAutoPool); \
                 stopConsole();\
                 return 0;

#ifdef  __cplusplus
}
#endif

#endif /*__RAY_FOUNDATION__*/