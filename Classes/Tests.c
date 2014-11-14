#include "RayFoundation/RayFoundation.h"

pointer mySandBoxAlloc(size_t size);

RSandBox* mySingleton(void) {
    static RSandBox *instance = nil;
    if(instance == nil) {
        instance = $(nil, c(RSandBox)), 4096, 128, RTrueMalloc, RTrueFree);
        instance->innerMallocPtr = mySandBoxAlloc;
        instance->allocationMode = RSandBoxAllocationModeStandart;
    }
    return instance;
}

pointer mySandBoxAlloc(size_t size) {
    pointer ptr = $(mySingleton(), m(malloc, RSandBox)), size);
//    RPrintf("RSandBox malloc in - %p, of pointer - %p\n", mySingleton(), ptr);
    return ptr;
}

pointer emptyRealloc(pointer ptr, size_t size) {
//    RPrintf("--- Realloc for %p, new size : %lu (bytes)\n", ptr, size);
    size_t oldSize = $(mySingleton(), m(sizeForPointer, RSandBox)), ptr);
    if(oldSize == 0) {
        RError("RSB. Bad pointer", mySingleton());
    }
    pointer newBuffer = mySandBoxAlloc(size);
    RMemMove(newBuffer, ptr, oldSize);
    return newBuffer;
}

int SandBoxTest() {
    initPointers();
    RReallocPtr = emptyRealloc;

    RByteArray *key = RBfromRCS(RS("Hello misha it's my new key ololo")); // key mustn't be not in sandbox

//    RPrintf("Sizeof pointer - %lu\n", sizeof(pointer));
    const size_t size = 10;
    size_t iterator;

    enableSandBoxMalloc(mySandBoxAlloc);  // enable our sandbox
    RArray *array = $(nil, c(RArray)), nil); // leaks
    forAll(iterator, size) {
        addObjectToRA(array, iterator);
    }
    if(mySingleton()->descriptorsInfo.from != 2) {
        disableSandBoxMalloc();
        RError("Bad sandbox count", mySingleton());
        return 1;
    }
    $(mySingleton(), m(XorCrypt, RSandBox)), key);
    $(mySingleton(), m(XorDecrypt, RSandBox)), key);
    if(mySingleton()->descriptorsInfo.from != 2) {
        disableSandBoxMalloc();
        RError("Bad sandbox decrypt", mySingleton());
        return 1;
    }
    disableSandBoxMalloc();         // disable sandbox

    $(mySingleton(), d(RSandBox)) );
    deallocator(mySingleton());
    return 0;
}

int RByteArrayTest() {
    size_t i;
    size_t j;
    for(i = 0; i < 1024; ++i) {
        RByteArray *array = makeRByteArray(i);
        if(i == 1023) {
            for(j = 0; j < 1024; ++j) {
                array->array[j] = (byte) j;
            }
            if(array->size != 1023) {
                RError("Bad array count", array);
                return 1;
            }
        }
        $(array, d(RByteArray)) );
        deallocator(array);
    }
    return 0;
}

int StringDictionaryTest() {
    // some key constant
    RCString *key = RS("Veider");

    // create dictionary
    RStringDictionary *dictionary = $(nil, c(RStringDictionary)) );

    // fill dictionary with some object-keys,
    // use RSC, cause we need copies of constant,
    // also use m(copy, RCString)
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Value"), RSC("Key"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Leia"), RSC("Han Solo"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Luke"), $(key, m(copy, RCString))) );
    if(dictionary->masterRDictionaryObject->keys->count != 3
            || dictionary->masterRDictionaryObject->values->count != 3) {
        RError("To little count in dictionary", dictionary);
        return 1;
    }

    // try once more, to check is it one-time-adding
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Value"), RSC("Key"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Leia"), RSC("Han Solo"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Luke"), $(key, m(copy, RCString))) );
    if(dictionary->masterRDictionaryObject->keys->count > 3
            || dictionary->masterRDictionaryObject->values->count > 3) {
        RError("Duplication of setBy key", dictionary);
        return 1;
    }

    // find some object for key
    RCString *object = $(dictionary, m(getObjectForKey, RStringDictionary)), key);
    if(object == nil) {
        RError("Error find object for key", dictionary);
        return 1;
    }

    // destructs, and delete pointer
    $(dictionary, d(RStringDictionary)) );
    deallocator(dictionary);
    return 0;
}

int StringArrayTest() {
    unsigned i;
    RArray *stringArray = makeRArray();
    stringArray->printerDelegate = p(RCString);
    stringArray->destructorDelegate = d(RCString);

    for(i = 0; i < 10; ++i) {
        addObjectToRA(stringArray, randomRCString());
    }
    if(stringArray->count != 10) {
        RError("String array count is not 10", stringArray);
        return 1;
    }

    $(stringArray, m(deleteObjects, RArray)), makeRRange(5, 4));
    if(stringArray->count != 6) {
        RError("String array delete objects error", stringArray);
        return 1;
    }

    return 0;
}

int RDictionaryTest(void){
    RDictionary *dictionary = makeRDictionary();
    size_t iterator;
    fromStartForAll(iterator, 1, 20){
        size_t value = iterator;
        size_t key = iterator;
        $(dictionary, m(setObjectForKey, RDictionary)), value, key);
        if(dictionary->keys->count != dictionary->values->count) {
            RError("Count of keys and values is not equal", dictionary);
            return 1;
        }
        if(dictionary->keys->count == 0
                || dictionary->keys->count == 0) {
            RError("Count of keys or values 0", dictionary);
            return 1;
        }
    }
    deleteRD(dictionary);
    return 0;
}

int RClassTableTest(void){
    size_t iterator;
    registerClassOnce("Luke");
    registerClassOnce("Dart");
    registerClassOnce("Leia");
    registerClassOnce("Han Solo");

    forAll(iterator, 1000) {
        registerClassOnce(randomRCString()->baseString);
    }
    // try once more, but here is only one record

    forAll(iterator, 1000) {
        registerClassOnce("Leia");
        registerClassOnce("Dart");
        registerClassOnce("Luke");
    }
    if(RCTSingleton->masterRArrayObject->count > 1008) {
        RError("RCTSingleton->masterRArrayObject->count > 108", RCTSingleton);
        return 1;
    }
    if(RCTSingleton->masterRArrayObject->count == 0) {
        RError("RCTSingleton->masterRArrayObject->count == 0", RCTSingleton);
        return 1;
    }
    char *checkName = "Han Solo";
//    RPrintf("Identifier of %s is - %lu \n", checkName, registerClassOnce(checkName));
    if(registerClassOnce(checkName) < 4) {
        RError("RCT. REGISTER", RCTSingleton);
        return 1;
    }
   $(RCTSingleton->masterRArrayObject, m(deleteObjects, RArray)), makeRRange(4, RCTSingleton->masterRArrayObject->count - 4));
    return 0;
}

int RClassNamePairTest(void){
    RClassNamePair *pair = $(nil, c(RClassNamePair)) );
    master(pair, RCString) = RS(toString(RClassNamePair));
    pair->idForClassName = 4;
    $(pair, d(RClassNamePair)) );
    deallocator(pair);
    return 0;
}

int RDynamicArrayTest(void){
    int i;
    RArray *dynamicArray = makeRArray();
    if(dynamicArray == nil) {
        RError("Make RArray error", dynamicArray);
        return 1;
    }
    dynamicArray->destructorDelegate = free;

    for (i = 0; i < 1000; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToRA(dynamicArray, a);
        if(dynamicArray->count != i + 1) {
            RError("RArray bad count", dynamicArray);
            return 1;
        }
    }

    sortRA(dynamicArray);

    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), makeRRange(80, 10));
    if(sub == nil) {
        RError("Subarray error", sub);
        return 1;
    }
    if(sub->count != 10) {
        RError("Subarray bad count", sub);
        return 1;
    }

    sizeToFitRA(sub);
    if(sub->freePlaces != 0) {
        RError("Subarray bad sizeToFit", sub);
        return 1;
    }

    deleteRA(dynamicArray);
    deallocator(sub);
    return 0;
}

void ComplexTest() {
    initPointers();
    if(!RDynamicArrayTest()
        && !RClassNamePairTest()
        && !RClassTableTest()
        && !RDictionaryTest()
        && !StringArrayTest()
        && !StringDictionaryTest()
        && !RByteArrayTest()
        && !SandBoxTest()) {
        RPrintf("All tests passed successfully\n");
    } else {
        RPrintf("TESTS ERROR!\n");
    }
}