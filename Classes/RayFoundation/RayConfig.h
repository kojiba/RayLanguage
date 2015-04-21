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

#include <RSystem.h>

#pragma mark Some config

//#define RAY_SHORT_DEBUG  // short trace for array and classTable
//#define RAY_CONSOLE_COLORS_ON // enable colored console messages
#define R_POOL_DETAILED      // enable to storage allocator thread id and malloced size

#pragma mark Errors

#define RAY_WARNINGS_ON // enable warnings defined in RSyntax.h, if this flag off - reduce bin/lib size
#define RAY_ERRORS_ON   // --//-- errors
//#define RAY_ASSERT_ON_ERRORS // enables assert on errors

#pragma mark Thread-Safety flags

#define RAY_ARRAY_THREAD_SAFE
//#define RAY_LIST_THREAD_SAFE // note: mutex type normal
//#define RAY_BUFFER_THREAD_SAFE
#define RAY_CLASS_TABLE_THREAD_SAFE
#define RAY_POOL_THREAD_SAFE
#define RAY_SAND_BOX_THREAD_SAFE

#ifdef RAY_ASSERT_ON_ERRORS
    #include <assert.h>
#endif

#endif /*__RAY_CONFIG_H__*/
