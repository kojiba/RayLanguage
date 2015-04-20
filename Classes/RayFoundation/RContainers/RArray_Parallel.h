/**
 * RArray_Parallel.h
 * Threaded optimisation for RArray functions.
 * Operations is synchronous.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2/27/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_ARRAY_PARALLEL_H__
#define __R_ARRAY_PARALLEL_H__

#include <RBasics.h>
#include <RArray.h>

method(RFindResult, findObjectParallel, RArray),    RCompareDelegate   *delegate);
method(void,        executeParallel,    RArray),    REnumerateDelegate *delegate);

#endif /*__R_ARRAY_PARALLEL_H__*/
