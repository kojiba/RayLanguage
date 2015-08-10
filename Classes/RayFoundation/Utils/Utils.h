/**
 * Utils.h
 * Some useful applicable of libRay.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 05/05/2015 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#include "RayFoundation/RContainers/RDictionary.h"
#include "RayFoundation/RContainers/RBuffer.h"

RDictionary * stringDictionaryFromFile(char *filename); // reads from file key-value (pairs like some.key = some.value)

// not thread safe
constMethod(RBuffer *, serializeToBufferDelegate, RArray), REnumerateDelegate *delegate);  // enumeration can be stopped, if size 0 than ignore, in context of enumerator must be size
