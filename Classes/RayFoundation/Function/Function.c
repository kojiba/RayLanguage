#include "Function.h"

Function *funcToByte(pointer begin, pointer end) {
    Function tempFunc;
    tempFunc.size = end - begin;

    if (tempFunc.size > 0) {

        tempFunc.body = malloc(tempFunc.size);
        tempFunc.pointer = begin;
        memcpy(tempFunc.body, ((byte *) begin), tempFunc.size);

    } else {

        tempFunc.pointer = NULL;
        tempFunc.body = NULL;
        tempFunc.size = 0;
    }

    return &tempFunc;
}