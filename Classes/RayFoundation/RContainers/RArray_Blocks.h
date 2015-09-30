/**
 * RArray_Blocks.h
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 9/29/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_ARRAY_BLOCKS__
#define __R_ARRAY_BLOCKS__

#include "RayFoundation/RayConfig.h"

#ifdef RAY_BLOCKS_ON

#include "RArray.h"

method(void,        deleteWithBlock,         RArray),    REnumerateBlock enumerator);
method(void,        deleteFastWithBlock,     RArray),    REnumerateBlock enumerator);
method(RArray *,    subarrayWithBlock,       RArray),    REnumerateBlock enumerator);
method(RFindResult, enumerateWithBlock,      RArray),    REnumerateBlock enumerator);
method(void,        quickSortWithBlock,      RArray),    size_t first, size_t last, byte (^comparator)(pointer, pointer));

#endif /* RAY_BLOCKS_ON */
#endif /*__R_ARRAY_BLOCKS__*/
