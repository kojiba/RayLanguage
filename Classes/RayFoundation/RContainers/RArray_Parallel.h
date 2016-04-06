/**
 * @file RArray_Parallel.h
 * @brief Threaded optimisation for RArray functions.
 *        Not uses RThreadPool.
 *        Operations is synchronous.
 *        Can't be used when RAY_EMBEDDED in ON in RayConfig.h
 *
 * @author Kucheruavyu Ilya (kojiba@protonmail.com)
 * @date 2/27/2015
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

#ifndef __R_ARRAY_PARALLEL_H__
#define __R_ARRAY_PARALLEL_H__

#include "RayFoundation/RBasics/RBasics.h"

#ifndef RAY_EMBEDDED
#include "RArray.h"

/**
 * RArray.findObjectWithDelegate parallel-analog.
 * Creates processorsCount() threads and separate work for each other using inner function privatePartFinder.
 * If one found some stops all another, or wait all threads until all workers end.
 * Not using system mutexes, or signals, or RThreadJoin.
 * Only lock/unlock inner RArray.mutex when threadsafety is ON.
 * Warnings when delegate in nil.
 */
method(RFindResult, findObjectParallel, RArray),    RCompareDelegate   *delegate);


/**
 * Used for make some same actions on objects in array.
 * Creates processorsCount() threads and separate work for each other using inner function privatePartExecuter.
 * Uses RThreadJoin to wait all threads finished.
 * Only lock/unlock inner RArray.mutex when threadsafety is ON.
 * Warnings when delegate in nil.
 */
method(void,        executeParallel,    RArray),    REnumerateDelegate *delegate);

#endif

#endif /*__R_ARRAY_PARALLEL_H__*/