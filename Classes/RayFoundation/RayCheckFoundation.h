/**
 * RayFoundationCheck.h
 * A ray of light in the realm of darkness.
 * Checking of working defines.
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

#ifndef __RAY_CHECK_FOUNDATION_H__
#define __RAY_CHECK_FOUNDATION_H__

    #if defined(toString) || defined(_TOSTRING) || defined(_concatenate) || defined(concatenate) || defined(initRClock) || defined(tickRClock) \
                          || defined(yes)       || defined(no)
            #error "Some of working RayFoundation defines are already defined!"
        #elif defined(class) || defined(discipleOf) || defined(endOf) || defined(protocol)
            #error "Some of core defines of RayFoundation are already defined!"
        #elif defined(method) || defined(staticMethod) || defined(virtualMethod)
            #error "Some of declaration defines of RayFoundation are already defined!"
        #elif defined(c) || defined(d) || defined(p) || defined(m) || defined(sm) || defined(singletonCall)
            #error "Some of function-call defines of RayFoundation are already defined!"
        #elif defined($) || defined(allocator) || defined(deallocator) || defined(master) || defined(forAll) || defined(fromStartForAll)
            #error "Some of core-function-call defines of RayFoundation are already defined!"
    #endif

#endif /*__RAY_CHECK_FOUNDATION_H__*/