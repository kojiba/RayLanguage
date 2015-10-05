/**
 * @file RayConfig.h
 * @brief Defines configuration flags (compile-time)
 * thread-safety of components, single platform additions,
 * compiler features, etc.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 * @date 1/19/2014
 * @par Ukraine Kharkiv
 *
 *//*
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 */

#ifndef __RAY_CONFIG_H__
#define __RAY_CONFIG_H__

#pragma mark Some config

//#define RAY_SHORT_DEBUG    ///< short trace for array and classTable
//#define RAY_CONSOLE_COLORS_ON    ///< enable colored console messages

#pragma mark Clang blocks

#if __has_extension(blocks)
//    #define RAY_BLOCKS_ON    ///< blocks extensions for containers
#endif

#pragma mark Embedded Cut

/**
 * Note:
 *
 * No network, no threads, minimal lib.
 * This macro understand no OS, and no stdlib.
 * Redefine to change allocation points (RAlloc, RReAlloc, RClearAlloc, RFree)
 * Redefine RPrintf to change output point.
 * See full list to redefine in RayBase.h
 */
//#define RAY_EMBEDDED

#ifdef RAY_EMBEDDED
    #warning Redefine RayBase.h DI macro and delete this warning
#else

//    #define RAY_LIBRARY_PRELOAD_ALLOCATOR    ///< not work in windows
    #define R_POOL_DETAILED      ///< to storage allocator thread id and malloced size
//    #define R_POOL_META_ALLOC    ///< POOL_DETAILED MUST BE ENABLED, to store string tip for pointer like metaAlloc(sizeInBytes, "some memory alloc"), only for RPool!

    #pragma mark Thread-Safety flags

    #define RAY_ARRAY_THREAD_SAFE
//    #define RAY_LIST_THREAD_SAFE    ///< note: mutex type normal
//    #define RAY_BUFFER_THREAD_SAFE
    #define RAY_CLASS_TABLE_THREAD_SAFE
    #define RAY_POOL_THREAD_SAFE
    #define RAY_SAND_BOX_THREAD_SAFE
#endif

#pragma mark Errors

#define RAY_WARNINGS_ON         ///< enable warnings defined in RSyntax.h (if off reduce bin/lib size)
#define RAY_ERRORS_ON           ///< enable errors   defined in RSyntax.h (if off reduce bin/lib size)
//#define RAY_ASSERT_ON_ERRORS    ///< enables assert on errors

#endif /*__RAY_CONFIG_H__*/
