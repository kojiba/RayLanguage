/**
 * RBasics.c
 * Some workers structures for Ray.
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

#include "RBasics.h"

#pragma mark RRange

RRange makeRRange(size_t from, size_t count) {
    RRange range;
    range.start = from;
    range.size = count;
    return range;
}

RRange makeRRangeTo(size_t from, size_t to) {
    RRange range;
    range.start = from;
    range.size = to - from;
    return range;
}

RCompareFlags compareRRange(RRange first, RRange second) {
    if(first.start == second.start) {
        if(first.size > second.size) {
            return longer;
        } else if(first.size < second.size) {
            return smaller;
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

rbool isInRange(RRange range, size_t value) {
    if(value >= range.start && value <= range.start + range.size) {
        return yes;
    } else {
        return no;
    }
}

rbool isOverlapping(RRange first, RRange second) {
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

#pragma mark RBounds

RBounds makeRBounds(char startSymbol, char endSymbol) {
    RBounds bounds;
    bounds.startSymbol = startSymbol;
    bounds.endSymbol = endSymbol;
    return bounds;
}

rbool isValueInBounds(RBounds bounds, char value) {
    if(value >= bounds.startSymbol && value <= bounds.endSymbol) {
        return yes;
    } else {
        return no;
    }
}

rbool compareRBounds(RBounds first, RBounds second) {
    if(first.startSymbol == second.startSymbol
            && first.endSymbol == second.endSymbol) {
        return yes;
    } else {
        return no;
    }
}

#pragma mark RCompareDelegate

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck){
    if(object->virtualCompareMethod != nil) {
        return object->virtualCompareMethod(object->etaloneObject, objectToCheck);
    } else if(object->etaloneObject == objectToCheck) {
        return equals;
    }
    #if RAY_SHORT_DEBUG == 1
            else {
                static pointer lastObject = nil;
                if(lastObject != object) {
                    lastObject = object;
                    RPrintf("Warning. RFD - %p virtual method is not implemented.\n", object);
                }
            }
    #endif
    return not_equals;
}