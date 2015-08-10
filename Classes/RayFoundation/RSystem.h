/**
 * RSystem.h
 * Small description about OS type.
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

#ifndef __R_SYSTEM_H__
#define __R_SYSTEM_H__

#include "RayConfig.h"

// Bit architecture
#if UINTPTR_MAX == 0xffff
    #define R16BIT
#elif UINTPTR_MAX == 0xffffff
    #define R24BIT
    #define RSystemBitVersion "24-bit "
#elif UINTPTR_MAX == 0xffffffff
    #define R32BIT
    #define RSystemBitVersion "32-bit "
#else
    #define R64BIT
    #define RSystemBitVersion "64-bit "
#endif

#ifndef RAY_EMBEDDED
    // OS type
    #ifdef _WIN32
       #define RSystemString "Windows "
    #endif

    #ifdef __APPLE__
        #define RSystemString "Apple "
        #include <TargetConditionals.h>

        #if TARGET_IPHONE_SIMULATOR
                #define RSystemDetails "iOS Simulator "
        #elif TARGET_OS_IPHONE
                #define RSystemDetails "iOS Device "
                #define iOSDevice
        #elif TARGET_OS_MAC
                #define RSystemDetails "OSX "
        #endif
    #endif

    #ifdef __linux
        // linux
        #define RSystemString  "Unix "
        #define RSystemDetails "Linux "
    #endif

    #ifdef __unix
        // unix-based
        #define RSystemString  "Unix "
    #endif

    #ifdef _POSIX_C_SOURCE
        // with posix support
        #define RPosixSupport "Posix Support"
    #endif

    // Make empty if undefined
    #ifndef RPosixSupport
        #define RPosixSupport
    #endif

    #ifndef RSystemDetails
        #define RSystemDetails
    #endif
#else
    // no OS
    #define RSystemString "Embedded "
    #define RSystemDetails
    #define RPosixSupport
#endif


#define RSystemType RSystemBitVersion RSystemString RSystemDetails RPosixSupport

#define RPrintCurrentSystem() RPrintLn("System : " RSystemType)

#ifdef RAY_EMBEDDED
    #define RPrintSystemInfo() RPrintCurrentSystem()
#else
    #define RPrintSystemInfo() RPrintCurrentSystem();\
                               RPrintf("Number of processors - %u \n",      processorsCount());\
                               RPrintf("Main tuid - %lu \n", (unsigned long)currentThreadIdentifier())
#endif

#endif /*__R_SYSTEM_H__*/