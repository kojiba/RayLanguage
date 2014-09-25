/**
 * @file RDictionary.c
 * @brief Realization of C dictionary(map) pair, in Ray additions.
 * @author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RDictionary.h"

constructor(RDictionary)){
    object = allocator(RDictionary);
    if(object == NULL) {
        RPrintf("");
    }
}

destructor(RDictionary){

}

printer(RDictionary){

}