/**
 * @file RStringDictionary.c
 * @brief Realization of C dictionary(map) for RCString, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RStringDictionary.h"

constructor(RStringDictionary)) {
    object = allocator(RStringDictionary);
    if(object != NULL) {
        master(object, RDictionary) = makeRDictionary();
        master(object, RCompareDelegate) = allocator(RCompareDelegate);
        if(master(object, RCompareDelegate) != NULL
                && master(object, RDictionary) != NULL) {
            master(object, RCompareDelegate)->virtualCompareMethod = m(compareWith, RCString);

            // set-up delegates destructors
            master(object, RDictionary)->keys->destructorDelegate = d(RCString);
            master(object, RDictionary)->values->destructorDelegate = d(RCString);

            // set-up delegates printers
            master(object, RDictionary)->keys->printerDelegate = p(RCString);
            master(object, RDictionary)->values->printerDelegate = p(RCString);
        }
    }
    return object;
}

destructor(RStringDictionary) {
    if(object != NULL) {
        $(master(object, RDictionary), d(RDictionary)) );
        deallocator(master(object, RDictionary));
        deallocator(master(object, RCompareDelegate));
    }
}