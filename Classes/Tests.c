#include <time.h>
#include <RayFoundation.h>

int RByteArrayTest(void) {
    size_t i;
    size_t j;
    for(i = 0; i < 32; ++i) {
        RByteArray *array = makeRByteArray(i);
        if(i == 31) {
            for(j = 0; j < 31; ++j) {
                array->array[j] = (byte) j;
            }
            if(array->size != 31) {
                RError("RByteArrayTest. Bad array size", array);
                return 1;
            }
        }
        deleter(array, RByteArray);
    }
    return 0;
}
void stringDeleter(pointer ptr){
    deleter(ptr, RCString);
}

int StringDictionaryTest(void) {
    // some key constant
    RCString *key = RS("Veider");

    // create dictionary
    RStringDictionary *dictionary = $(nil, c(RStringDictionary)) );
    master(dictionary, RDictionary)->values->destructorDelegate = stringDeleter;
    master(dictionary, RDictionary)->keys->destructorDelegate = stringDeleter;

    // fill dictionary with some object-keys,
    // use RSC, cause we need copies of constant,
    // also use m(copy, RCString)
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Value"), RSC("Key"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Leia"), RSC("Han Solo"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Luke"), $(key, m(copy, RCString))) );

    if(dictionary->masterRDictionaryObject->keys->count > 3
            || dictionary->masterRDictionaryObject->values->count > 3) {
        RError("StringDictionaryTest. Duplication of setBy key", dictionary);
        return 1;
    }

    // find some object for key
    RCString *object = $(dictionary, m(getObjectForKey, RStringDictionary)), key);
    if(object == nil) {
        RError("StringDictionaryTest. Error find object for key", dictionary);
        return 1;
    } else {
        deallocator(key);
    }

    // destructs, and delete pointer
    deleter(dictionary, RStringDictionary);
    return 0;
}



int StringArrayTest(void) {
    unsigned i;
    RArray *stringArray = makeRArray();
//    stringArray->printerDelegate = p(RCString);
    stringArray->destructorDelegate = stringDeleter;

    for(i = 0; i < 10; ++i) {
        addObjectToRA(stringArray, randomRCString());
    }
    if(stringArray->count != 10) {
        RError("String array size is not 10", stringArray);
        return 1;
    }

    $(stringArray, m(deleteObjects, RArray)), makeRRange(5, 4));
    if(stringArray->count != 6) {
        RError("String array delete objects error", stringArray);
        return 1;
    }
    deleter(stringArray, RArray);

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
            RError("Tests. RDictionaryCount of keys and values is not equal.", dictionary);
            return 1;
        }
        if(dictionary->keys->count == 0
                || dictionary->keys->count == 0) {
            RError("Tests. RDictionaryCount of keys or values 0.", dictionary);
            return 1;
        }
    }
    deleter(dictionary, RDictionary);
    return 0;
}

int RClassTableTest(void){
    size_t iterator;
    registerClassOnce("Luke");
    registerClassOnce("Dart");
    registerClassOnce("Leia");
    registerClassOnce("Han Solo");
    // try once more, but here is only one record

    forAll(iterator, 100) {
        registerClassOnce("Leia");
        registerClassOnce("Dart");
        registerClassOnce("Luke");
    }

    if(RCTSingleton->masterRArrayObject->count > 20) {
        RError("RCTSingleton->masterRArrayObject->size > 8", RCTSingleton);
        return 1;
    }
    if(RCTSingleton->masterRArrayObject->count == 0) {
        RError("RCTSingleton->masterRArrayObject->size == 0", RCTSingleton);
        return 1;
    }
    char *checkName = "Leia";
//    RPrintf("Identifier of %s is - %lu \n", checkName, registerClassOnce(checkName));
    size_t id = $(RCTSingleton, m(getIdentifierByClassName, RClassTable)), checkName);
    if(id < 4) {
        printRCTS;
        RError("Test. RClassTable. Bad id for registered.", RCTSingleton);
        return 1;
    }
    return 0;
}

int RClassNamePairTest(void){
    RClassNamePair *pair = $(nil, c(RClassNamePair)) );
    if(pair != nil) {
        $(master(pair, RCString), m(setConstantString, RCString)), "RClassNamePairTest");
        pair->idForClassName = 4;
        deleter(pair, RClassNamePair);
    }
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

    for (i = 0; i < 22; ++i) {
        char *a = RAlloc(sizeof(char) * 8);
        if(a != nil) {
            RMemCpy(a, "Hello  ", sizeof("Hello  "));
            a[6] = (char) (i % 10 + 48);
            addObjectToRA(dynamicArray, a);
        }
        if(dynamicArray->count != i + 1) {
            RError("RArray bad size", dynamicArray);
            return 1;
        }
    }

    sortRA(dynamicArray);

    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), makeRRange(10, 10));
    if(sub == nil) {
        RError("Subarray error.", sub);
        return 1;
    }
    sub->destructorDelegate = nil;
    if(sub->count != 10) {
        RError("Subarray bad size.", sub);
        return 1;
    }
    sizeToFitRA(sub);
    if(sub->freePlaces != 0) {
        RError("Subarray bad sizeToFit.", sub);
        return 1;
    }

    deleter(dynamicArray, RArray);
    deleter(sub, RArray);
    return 0;
}

int RBufferTest(void) {
    size_t iterator;
    size_t sum = 0;
    size_t first_size = 0;
    RBuffer *buffer = $(nil, c(RBuffer)));

    forAll(iterator, 20) {
        RCString *temp = randomRCString();
        $(buffer, m(addData, RBuffer)), temp->baseString, temp->size + 1);
        sum += temp->size + 1;
        if(iterator == 0) {
            first_size = temp->size;
        }
        if((buffer->count != iterator + 1) || buffer->totalPlaced != sum) {
            RError("RBuffer. Test error, bad size or size", buffer);
            return -1;
        }
        deleter(temp, RCString);
    }
    char *temp = $(buffer, m(getDataCopy, RBuffer)), 0);
    if(RStringLenght(temp) != first_size) {
        RError("RBuffer. Bad getDataCopy", buffer);
    }
    deleter(buffer, RBuffer);
    deallocator(temp);
    return 0;
}

size_t threadCounter;

pointer threadFunction(pointer pVoid) {
    ++threadCounter;
    ++threadCounter;
    ++threadCounter;
    ++threadCounter;
    ++threadCounter;
    return nil;
}

int RThreadTest(void) {
    RThread *thread = $(nil, c(RThread)), nil, threadFunction, nil);
    ++threadCounter;
    $(thread, m(join, RThread)));
    if(threadCounter != 6) {
        RError("RThread. Test error, bad counter.", thread);
        return -1;
    }
    deleter(thread, RThread);
    return 0;
}

void ComplexTest() {
    srand((unsigned int) time(nil));
    threadCounter = 0;
    RPrintCurrentSystem();
    if(
           !RDynamicArrayTest()
        && !RClassNamePairTest()
        && !RClassTableTest()
        && !RDictionaryTest()
        && !StringArrayTest()
        && !StringDictionaryTest()
        && !RByteArrayTest()
        && !RBufferTest()
        && !RThreadTest()
    ) {
        RPrintf("All tests passed successfully\n");
    } else {
        RError("TESTS ERROR!", nil);
    }
}