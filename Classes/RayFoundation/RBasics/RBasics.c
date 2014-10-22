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

RRange makeRRange(uint64_t from, uint64_t count) {
    RRange range;
    range.from = from;
    range.count = count;
    return range;
}

RRange makeRRangeTo(uint64_t from, uint64_t to) {
    RRange range;
    range.from = from;
    range.count = to - from;
    return range;
}

#pragma mark RBounds

RBounds makeRBounds(char startSymbol, char endSymbol) {
    RBounds bounds;
    bounds.startSymbol = startSymbol;
    bounds.endSymbol = endSymbol;
    return bounds;
}

#pragma mark RCompareDelegate

method(RCompareFlags, checkObject, RCompareDelegate), pointer objectToCheck){
    if(object->virtualCompareMethod != NULL) {
        return object->virtualCompareMethod(object->etaloneObject, objectToCheck);
    } else if(object->etaloneObject == objectToCheck) {
        return equals;
    }
    #if RAY_SHORT_DEBUG == 1
            else {
                static pointer lastObject = NULL;
                if(lastObject != object) {
                    lastObject = object;
                    RPrintf("Warning. RFD - %p virtual method is not implemented.\n", object);
                }
            }
    #endif
    return not_equals;
}