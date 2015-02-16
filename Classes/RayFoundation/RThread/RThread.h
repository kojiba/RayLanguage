/**
 * RThread.h
 * Simple compile-based wrapper posix and winapi thread dependency.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12/16/14 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R__THREAD_H__
#define __R__THREAD_H__

#include <RSyntax.h>
#include <RThreadNative.h>

// base
RMutexDescriptor mutexWithType(unsigned short mutexType);

// wrapper
class(RThread)
    RThreadDescriptor descriptor;
endOf(RThread)

constructor (RThread),
        RThreadAttributes *attributes,
        RThreadFunction function,
        pointer argument);

printer     (RThread);

method(void, cancel,  RThread));
method(void, join,    RThread));

#endif /*__R__THREAD_H__*/
