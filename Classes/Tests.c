#include <time.h>
#include <RayFoundation/RayFoundation.h>

int RDataTest(void) {
    size_t i;
    size_t j;
    for(i = 0; i < 32; ++i) {
        RData *array = makeRDataAllocated(i);
        if(i == 31) {
            for(j = 0; j < 31; ++j) {
                array->data[j] = (byte) j;
            }
            RAY_TEST(array->size != 31, "RDataTest. Bad array size", -1);
        }
        deleter(array, RData);
    }
    return 0;
}

int StringArrayTest(void) {
    unsigned i;
    RArray *stringArray = makeRArray();
    stringArray->destructorDelegate = (DestructorDelegate) RDataDeleter;
    for(i = 0; i < 10; ++i) {
         $(stringArray, m(addObject, RArray)), stringWithFormat("Temp string %i", i));
    }
    RAY_TEST(stringArray->count != 10, "String array size is not 10", -1);

    $(stringArray, m(deleteObjects, RArray)), makeRRange(5, 4));
    RAY_TEST(stringArray->count != 6, "String array delete objects error", -2);
    deleter(stringArray, RArray);

    return 0;
}

int RDictionaryTest(void){
    RDictionary *dictionary = makeRDictionary();
    size_t iterator;
    fromStartForAll(iterator, 1, 20){
        size_t value = iterator;
        size_t key = iterator;
        $(dictionary, m(setObjectForKey, RDictionary)), (pointer) value, (pointer) key);
        RAY_TEST(dictionary->keys->count != dictionary->values->count, "Tests. RDictionaryCount of keys and values is not equal.", -1);
        RAY_TEST(dictionary->keys->count == 0 || dictionary->values->count == 0, "Tests. RDictionaryCount of keys or values 0.", -2);
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

    forAll(iterator, 20) {
        registerClassOnce("Leia");
        registerClassOnce("Dart");
        registerClassOnce("Luke");
    }

    RAY_TEST(RCTSingleton->masterRArrayObject->count > 20, "RCTSingleton->masterRArrayObject->size > 20", -1);
    RAY_TEST(RCTSingleton->masterRArrayObject->count == 0, "RCTSingleton->masterRArrayObject->size == 0", -2);
    char *checkName = "Leia";
    size_t id = $(RCTSingleton, m(getIdentifierByClassName, RClassTable)), checkName);
    RAY_TEST(id < 4, "RClassTable. Bad id for registered.", -3);
    return 0;
}

int RClassNamePairTest(void){
    RClassNamePair *pair = $(nil, c(RClassNamePair)) );
    if(pair != nil) {
        $(master(pair, RString), m(setConstantString, RString)), "RClassNamePairTest");
        pair->idForClassName = 4;
        deleter(pair, RClassNamePair);
    }
    return 0;
}

int RDynamicArrayTest(void){
    int i;
    RArray *dynamicArray = makeRArray();
    RAY_TEST(dynamicArray == nil, "Make RArray error", -1);
    dynamicArray->destructorDelegate = free;

    for (i = 0; i < 22; ++i) {
        char *a = RAlloc(sizeof(char) * 8);
        if(a != nil) {
            RMemCpy(a, "Hello  ", sizeof("Hello  "));
            a[6] = (char) (i % 10 + 48);
             $(dynamicArray, m(addObject, RArray)), a);
        }
        RAY_TEST(dynamicArray->count != i + 1, "RArray bad size", -2);
    }

    $(dynamicArray, m(sort, RArray)));

    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), makeRRange(10, 10));
    RAY_TEST(sub == nil, "Subarray error.", -3);
    sub->destructorDelegate = nil;
    RAY_TEST(sub->count != 10, "Subarray bad size.", -4);
    $(sub, m(sizeToFit, RArray)));
    RAY_TEST(sub->freePlaces != 0, "Subarray bad sizeToFit.", -5);

    deleter(dynamicArray, RArray);
    deleter(sub, RArray);
    return 0;
}

int RBufferTest(void) {
    size_t iterator;
    size_t sum = 0;
    RBuffer *buffer = $(nil, c(RBuffer)));

    forAll(iterator, 20) {
        RString *temp = randomASCIIString();
        $(buffer, m(addBytes, RBuffer)), temp->data, temp->size);
        sum += temp->size;
        RAY_TEST(((buffer->count != iterator + 1) || buffer->totalPlaced != sum),
                "RBuffer. Test error, bad size or size", -1);

        deleter(temp, RString);
    }
    byte *temp = $(buffer, m(getDataCopy, RBuffer)), 0);

    RAY_TEST(temp == nil, "RBuffer. Bad getDataCopy", -2);

    deleter(buffer, RBuffer);
    deallocator(temp);
    return 0;
}


rbool checkObjectRListFinder(pointer context, pointer data, size_t index) {
    if(index == 2) {
        return no;
    }
    return yes;
}

int RListTest(void) {
    size_t iterator;
    REnumerateDelegate finder;
    // link virtual
    finder.virtualEnumerator = checkObjectRListFinder;

    RList *list = constructorOfRList(nil);

#define listObjectsCount 40

    forAll(iterator, listObjectsCount) {
        $(list, m(addHead, RList)), (pointer) iterator);
    }
    RAY_TEST(list->count != listObjectsCount, "RList. Bad add head.", -1);

    RList* result = $(list, m(subList, RList)), makeRRange(5, 5));
    RAY_TEST(!result, "RList. Bad sublist.", -2);
    RAY_TEST(result->count != 5, "RList. Bad sublist count.", -3);

    RFindResult founded = $(result, m(enumerate, RList)), &finder, yes);

    RAY_TEST(founded.object == nil, "RList. Bad founded object.", -4);
    RAY_TEST(founded.index == list->count, "RList. Bad finded index.", -5);

    deleter(result, RList);

    $(list, m(deleteObjects, RList)), makeRRange(10, 10));
    RAY_TEST(list->count != listObjectsCount - 10, "RList. Bad count on delete.", -6);

    RArray *array = $(list, m(toRArray, RList)));
    RAY_TEST(!array, "RList. Bad array from list", -7);
    RAY_TEST(list->count != array->count && array->count != listObjectsCount - 10, "RList. Bad count of array from list.", -8);

    deleter(array, RArray);
    deleter(list, RList);
    return 0;
}

#define TEST_COUNT 10

RArray *arrayTest;

pointer func1(pointer arg) {
    int i;
    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RCS((char*)arg));
    }
    return 0;
}

int RThreadTest(void) {
#if !defined(RAY_EMBEDDED) && defined(RAY_ARRAY_THREAD_SAFE)
    arrayTest = makeRArray();
    int i;
    arrayTest->destructorDelegate = free;
    RThread thread1;
    RThreadCreate(&thread1, nil, (RThreadFunction) func1, "1 thread");

    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RCS("main"));
    }

    $(thread1, RThreadJoin));

    RAY_TEST(arrayTest->count != 2 * TEST_COUNT, "RThread bad array count.", -1);
    deleter(arrayTest, RArray);
#endif
    return 0;
}

int RStringTest(void) {
    RString *testString = RSC(
        "Warcraft II in gameplay and in attention to personality and storyline." //11
        "In 1996 Blizzard announced Warcraft Adventures: Lord of the Clans, an adventure game." //13
        "But canceled the game in 1998." // 6
        "Warcraft III: Reign of Chaos, released in 2002, used parts of Warcraft Adventures characters and storyline and extended the gameplay used in Warcraft II." //24
    );

    RArray *words = $(testString, m(substringsSeparatedBySymbols, RString)), RS(".,:; /-!@=+?\n\t&"));

    RAY_TEST(words == nil, "RString. Words array wasn't created.", -1);
    RAY_TEST(words->count != 54, "RString. Bad text words count.", -2);


    deleter(words, RArray);

    $(testString, m(deleteAllCSubstrings, RString)), "\n");
    $(testString, m(removeRepetitionsOf, RString)), ' ');

    RArray *sents = $(testString, m(substringsSeparatedByCSymbols, RString)), ".");
    RAY_TEST(sents == nil, "RString. Sentances array wasn't created.", -3);
    if(sents != nil) {
        RAY_TEST(sents->count != 4, "RString. Bad text sentances count.", -4);
        deleter(sents, RArray);
    }
    deleter(testString, RString);
    return 0;
}

int RThreadPoolTest(void) {
#if !defined(RAY_EMBEDDED) && defined(RAY_ARRAY_THREAD_SAFE)
    arrayTest = makeRArray();
    int i;
    arrayTest->destructorDelegate = free;

    RThreadPool * pool = c(RThreadPool)(nil);
    $(pool, m(setDelegateFunction, RThreadPool)), func1);

    forAll(i, TEST_COUNT) {
        $(pool, m(addWithArg, RThreadPool)), "POOL self deletes", yes);
    }

    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RCS("main"));
    }

    $(pool, m(joinSelfDeletes, RThreadPool)));

    RAY_TEST(arrayTest->count != TEST_COUNT * (TEST_COUNT + 1), "RThreadPool self-deletes bad array count.", -1);

    deleter(arrayTest, RArray);
    deleter(pool, RThreadPool);

    arrayTest = makeRArray();

    arrayTest->destructorDelegate = free;

    pool = c(RThreadPool)(nil);
    $(pool, m(setDelegateFunction, RThreadPool)), func1);

    forAll(i, TEST_COUNT) {
        $(pool, m(addWithArg, RThreadPool)), "POOL simple", no);
    }

    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RCS("main"));
    }
    $(pool, m(join, RThreadPool)));
    RAY_TEST(arrayTest->count != TEST_COUNT * (TEST_COUNT + 1), "RThreadPool bad array count.", -2);

    deleter(arrayTest, RArray);
    deleter(pool, RThreadPool);


#endif
    return 0;
}


int RByteApiTest(void) {
#define dataSizeTest 1024
    size_t iterator;
    RData *data = makeRDataAllocated(dataSizeTest);
    RAY_TEST(data->size != dataSizeTest, "RByteApiTest bad array count.", -1);
    flushAllToByte(data->data, dataSizeTest, 1);

    forAll(iterator, dataSizeTest) {
        RAY_TEST(data->data[iterator] != 1, "RByteApiTest bad flushed array value (1).", -2);
    }

    forAll(iterator, dataSizeTest) {
        data->data[iterator] = (byte) (iterator % 16);
    }

    RAY_TEST(data->data[17] != 1, "RByteApiTest bad array setted.", -3);


    // {0, 1, 2}
    RData *separator = makeRDataBytes(subArrayInRange(data->data, data->size, makeRRange(0, 3)), 3);

    RAY_TEST(separator == nil, "RByteApiTest bad subArrayInRange", -4);
    RAY_TEST(separator->data == nil, "RByteApiTest bad subArrayInRange", -5);
    RAY_TEST(separator->size != 3, "RByteApiTest bad subArrayInRange", -6);

    replaceBytesWithBytes(data->data, &data->size, separator->data, separator->size, nil, 0);

    RAY_TEST(data->size != dataSizeTest - dataSizeTest / 16 * 3, "RByteApiTest bad size after replaceBytesWithBytes", -7);

    forAll(iterator, data->size) {
        if(iterator % 13 == 0) {
            RAY_TEST(data->data[iterator] != 3, "RByteApiTest bad byte at index after replaceBytesWithBytes", -8);
        }
    }

    replaceByteWithByte(data->data, data->size, 3, 4);

    forAll(iterator, data->size) {
        if(iterator % 13 == 0) {
            RAY_TEST(data->data[iterator] != 4, "RByteApiTest bad byte at index after replaceByteWithByte", -9);
        }
    }


    byte sub[] = {0x4, 0x4};
    deleteAllSubArrays(data->data, &data->size, sub, 2);

    RAY_TEST(data->size != dataSizeTest - dataSizeTest / 16 * (3 + 2), "RByteApiTest bad size after deleteAllSubArrays", -10);

    deleter(separator, RData);
    deleter(data, RData);

    return 0;
}

void ComplexTest() {
    int code;
    int testNumber = 0;
    srand((unsigned int) time(nil));

//    RPrintSystemInfo();

    if(
           !(++testNumber, code = RDynamicArrayTest())
        && !(++testNumber, code = RListTest())
        && !(++testNumber, code = RClassNamePairTest())
        && !(++testNumber, code = RClassTableTest())
        && !(++testNumber, code = RDictionaryTest())
        && !(++testNumber, code = StringArrayTest())
        && !(++testNumber, code = RDataTest())
        && !(++testNumber, code = RBufferTest())
        && !(++testNumber, code = RStringTest())
        && !(++testNumber, code = RThreadTest())
        && !(++testNumber, code = RThreadPoolTest())
        && !(++testNumber, code = RByteApiTest())
    ) {
//        RPrintLn("All tests passed successfully\n");
    } else {
        RError("TESTS ERROR!", nil);
        RPrintf("TESTS ERROR, code: %d , testNo: %d!\n\n", code, testNumber);
    }
}