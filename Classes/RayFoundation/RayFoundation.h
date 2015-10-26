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
#include "RContainers/RArray_Blocks.h"
#include "RContainers/RArray_Parallel.h"
#include "RContainers/RList.h"
#include "RContainers/RBuffer.h"
#include "RContainers/RDictionary.h"

// Strings
#include "RString/RString.h"
#include "RString/RString_Char.h"
#include "RString/RString_Numbers.h"
#include "RString/RString_UTF8.h"
#include "RString/RString_Consts.h"
#include "RString/RString_File.h"

// Memory operations
#include "RMemoryOperations/RByteOperations.h"
#include "RMemoryOperations/RData.h"
#include "RMemoryOperations/RSandBox.h"
#include "RMemoryOperations/RAutoPool.h"

// Encoding
#include "REncoding/RBase64.h"
#include "REncoding/PurgeEvasionUtilsRay.h"

// Networking
#include "RNetwork/RSocket.h"
#include "RNetwork/RTCPHandler.h"

// Threads
#include "RThread/RThread.h"
#include "RThread/RThreadPool.h"

// Others
#include "RClassTable/RClassTable.h"
#include "Utils/Utils.h"
#include "Utils/PurgeEvasionConnection.h"

#ifdef RAY_LIBRARY_PRELOAD_ALLOCATOR
    #define endRay() deleter(stringConstantsTable(), RDictionary);\
                     deleter(RCTSingleton, RClassTable); \
                     stopConsole();\
                     return 0
#else
    #ifdef RAY_BLOCKS_ON
        #define endRay() deleter(stringConstantsTable(), RDictionary);\
                         deleter(RCTSingleton, RClassTable); \
                         deleter(singleBlockPool(), RAutoPool); \
                         p(RAutoPool)(RPool); \
                         deleter(RPool, RAutoPool); \
                         stopConsole();\
                         return 0
    #else
        #define endRay() deleter(stringConstantsTable(), RDictionary);\
                         deleter(RCTSingleton, RClassTable); \
                         p(RAutoPool)(RPool); \
                         deleter(RPool, RAutoPool); \
                         stopConsole();\
                         return 0
    #endif
#endif

#ifdef  __cplusplus
}
#endif

#endif /*__RAY_FOUNDATION__*/