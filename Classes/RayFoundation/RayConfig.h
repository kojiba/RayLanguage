/**
 * RayBase.h
 * A ray of light in the realm of darkness.
 * Defines flags to config
 * thread-safety of components, console-debug, etc.
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

#ifndef __RAY_CONFIG_H__
#define __RAY_CONFIG_H__

#define RAY_WARNINGS_ON // enable warnings defined in RSyntax.h
#define RAY_ERRORS_ON   // --//-- errors

//#define RAY_SHORT_DEBUG  // short trace for array and classTable

// thread-safety flags
#define RAY_ARRAY_THREAD_SAFE
#define RAY_CLASS_TABLE_THREAD_SAFE
#define RAY_POOL_THREAD_SAFE
#define RAY_BUFFER_THREAD_SAFE

//#define RAY_ASSERT_ON_ERRORS // enables assert on errors
#ifdef RAY_ASSERT_ON_ERRORS
    #include <assert.h>
#endif

#endif /*__RAY_CONFIG_H__*/
