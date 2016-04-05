/**
 * RBasics.c
 * Some workers structures for Ray.
 * Author Kucheruavyu Ilya (kojiba@protonmail.com)
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

#include "RBasics.h"

#pragma mark RRange

inline RRange makeRRange(size_t from, size_t count) {
    RRange range;
    range.start = from;
    range.size = count;
    return range;
}

inline RRange makeRRangeTo(size_t from, size_t to) {
    RRange range;
    range.start = from;
    range.size = to - from;
    return range;
}

inline RCompareFlags compareRRange(RRange first, RRange second) {
    if(first.start == second.start) {
        if(first.size > second.size) {
            return longer;
        } else if(first.size < second.size) {
            return shorter;
        } else {
            return equals;
        }
    } else {
        if(first.start > second.start) {
            return bigger;
        } else {
            return smaller;
        }
    }
}

inline rbool isInRange(RRange range, size_t value) {
    if(value >= range.start && value <= range.start + range.size) {
        return yes;
    } else {
        return no;
    }
}

inline rbool isOverlappingRRange(RRange first, RRange second) {
    if((second.start > (first.start + first.size))
            || ((second.start + second.size) < first.start)) {
        return no;
    } else {
        return yes;
    }
}

printer(RRange) {
    RPrintf("[ %lu ; %lu ]", object->start, object->start + object->size);
}

inline RCompareFlags defaultComparator(pointer first, pointer second) {
    if(first == second) {
        return equals;
    } else {
        return not_equals;
    }
}