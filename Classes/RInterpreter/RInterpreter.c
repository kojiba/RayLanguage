/**
 * RInterpreter.c
 * A ray of light in the realm of darkness.
 * Interpreter of Ray language to pure C.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 12.11.2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RInterpreter.h"
#include "../RayFoundation/RClassTable/RClassTable.h"

constructor(RInterpreter)) {
    object = allocator(RInterpreter);
    if(object != nil) {
        object->classId = registerClassOnce(toString(RInterpreter));
    }
    return object;
}

destructor(RInterpreter) {
    if(object->sourceFileString != nil) {
        deleteRCS(object->sourceFileString);
    }
}

singleton(RInterpreter) {
    static RInterpreter *instance = nil;
    if(instance == nil) {
        instance = $(nil, c(RInterpreter)));
    }
    return instance;
}

method(RCString*, fileNameFromSourceName, RInterpreter), const RCString *sourceFileName) {
    RCString *oldExtension = RS(".ray");
    RCString *fileExtension = $(sourceFileName, m(substringInRange, RCString)), makeRRange(sourceFileName->size - oldExtension->size, oldExtension->size));
    RCString *newExtension = RS("c");
    if($(fileExtension, m(compareWith, RCString)), oldExtension) != equals) {
        RError("RInterpreter. Filetype is not a \'.ray\'", object);
        return nil;
    }
    RCString *result = $(sourceFileName, m(copy, RCString)) );
    $(result, m(deleteInRange, RCString)), makeRRange(sourceFileName->size - oldExtension->size + 1, oldExtension->size - 1));
    $(result, m(concatenate, RCString)), newExtension);
    deallocator(oldExtension);
    deallocator(newExtension);
    return result;
}

method(RCString*, convertRayToC, RInterpreter), const char *sourceFileName) {
    RCString *resultName = $(object, m(fileNameFromSourceName, RInterpreter)), RS(sourceFileName));
    if(resultName != nil) {
        if(object->sourceFileString != nil) {
            deleteRCS(object->sourceFileString);
        }
        object->sourceFileString = RCStringFromFile(sourceFileName);
        // parse and save
    }
    return nil;
}