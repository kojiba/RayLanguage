/**
 * @file RArray_Blocks.h
 * @brief Block category to RArray operations.
 *        For use it uncomment RAY_BLOCKS_ON in RayConfig.h.
 *
 * @author Kucheruavyu Ilya (kojiba@protonmail.com)
 * @date 9/29/2015
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

#ifndef __R_ARRAY_BLOCKS__
#define __R_ARRAY_BLOCKS__

#include "RayFoundation/RayConfig.h"

#ifdef RAY_BLOCKS_ON

#include "RArray.h"


/**
 * Destroy object at index where enumerator block returns yes, and use RArray.shift
 */
method(void,        deleteWithBlock,         RArray),    REnumerateBlock enumerator);

/**
 * Full RArray.deleteWithPredicate block-analog.
 */
method(void,        deleteFastWithBlock,     RArray),    REnumerateBlock enumerator);

/**
 * Full RArray.subarrayWithPredicate block-analog.
 */
method(RArray *,    subarrayWithBlock,       RArray),    REnumerateBlock enumerator);

/**
 * Full RArray.enumerate block-analog.
 */
method(RFindResult, enumerateWithBlock,      RArray),    REnumerateBlock enumerator);

/**
 * Full RArray.quickSortWithDelegate block-analog.
 * Note: recursive, NOT check bounds.
 */
method(void,        quickSortWithBlock,      RArray),    size_t first, size_t last, byte (^comparator)(pointer, pointer));

#endif /* RAY_BLOCKS_ON */
#endif /*__R_ARRAY_BLOCKS__*/
