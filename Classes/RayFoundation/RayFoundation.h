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
#include <RSystem.h>
#include <RSyntax.h>
#include <RColors.h>
#include <RBasics.h>

// Containers
#include <RArray.h>
#include <RList.h>
#include <RBuffer.h>
#include <RDictionary.h>

// Strings
#include <RCString.h>
#include <RCString_Char.h>
#include <RCString_Numbers.h>
#include <RString.h>
#include <RString_Consts.h>

// Memory operations
#include <RByteOperations.h>
#include <RSandBox.h>
#include <RAutoPool.h>

// Encoding
#include <RBase64.h>

// Networking
#include <RSocket.h>

// Threads
#include <RThread.h>
#include <RThreadPool.h>

// Others
#include <RClassTable.h>
#include <Utils.h>

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