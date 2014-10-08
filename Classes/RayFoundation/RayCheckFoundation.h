/**
 * RayFoundationCheck.h
 * A ray of light in the realm of darkness.
 * Checking of working defines.
 * Created (post-factum) 2 Oct 2014
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __RAY_CHECK_FOUNDATION_H__
#define __RAY_CHECK_FOUNDATION_H__

#if defined(toString) || defined(_TOSTRING) || defined(_concatenate) || defined(concatenate) || defined(initRClock) || defined(tickRClock)
        #error "Some of working RayFoundation defines are already defined"
    #elif defined(class) || defined(discipleOf) || defined(endOf) || defined(protocol)
        #error "Some of core defines of RayFoundation are already defined"
    #elif defined(method) || defined(staticMethod) || defined(virtualMethod)
        #error "Some of declaration defines of RayFoundation are already defined"
    #elif defined(c) || defined(d) || defined(p) || defined(m) || defined(sm) || defined(singletonCall)
        #error "Some of function-call defines of RayFoundation are already defined"
    #elif defined($) || defined(allocator) || defined(deallocator) || defined(master) || defined(forAll) || defined(fromStartForAll)
        #error "Some of core-function-call defines of RayFoundation are already defined"
    #endif

#endif /*__RAY_CHECK_FOUNDATION_H__*/