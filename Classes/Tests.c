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
            RAY_TEST(array->size != 31, "RByteArrayTest. Bad array size", -1);
        }
        deleter(array, RByteArray);
    }
    return 0;
}

int StringDictionaryTest(void) {
    // some key constant
    RCString *key = RS("Veider");

    // create dictionary
    RStringDictionary *dictionary = $(nil, c(RStringDictionary)) );
    master(dictionary, RDictionary)->values->destructorDelegate = (void (*)(pointer)) stringDeleter;
    master(dictionary, RDictionary)->keys->destructorDelegate = (void (*)(pointer)) stringDeleter;

    // fill dictionary with some object-keys,
    // use RSC, cause we need copies of constant,
    // also use m(copy, RCString)
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Value"), RSC("Key"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Leia"), RSC("Han Solo"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Luke"), $(key, m(copy, RCString))) );

    RAY_TEST(dictionary->masterRDictionaryObject->keys->count > 3
            || dictionary->masterRDictionaryObject->values->count > 3,
            "StringDictionaryTest. Duplication of setBy key", -1);

    // find some object for key
    RCString *object = $(dictionary, m(getObjectForKey, RStringDictionary)), key);
    RAY_TEST(object == nil, "StringDictionaryTest. Error find object for key", -2)
    else {
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
    stringArray->destructorDelegate = (void (*)(pointer)) stringDeleter;

    for(i = 0; i < 10; ++i) {
        addObjectToRA(stringArray, randomRCString());
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

    forAll(iterator, 100) {
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
        $(master(pair, RCString), m(setConstantString, RCString)), "RClassNamePairTest");
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
            addObjectToRA(dynamicArray, a);
        }
        RAY_TEST(dynamicArray->count != i + 1, "RArray bad size", -2);
    }

    sortRA(dynamicArray);

    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), makeRRange(10, 10));
    RAY_TEST(sub == nil, "Subarray error.", -3);
    sub->destructorDelegate = nil;
    RAY_TEST(sub->count != 10, "Subarray bad size.", -4);
    sizeToFitRA(sub);
    RAY_TEST(sub->freePlaces != 0, "Subarray bad sizeToFit.", -5);

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
        RAY_TEST(((buffer->count != iterator + 1) || buffer->totalPlaced != sum),
                "RBuffer. Test error, bad size or size", -1);

        deleter(temp, RCString);
    }
    char *temp = $(buffer, m(getDataCopy, RBuffer)), 0);

    RAY_TEST(RStringLength(temp) != first_size, "RBuffer. Bad getDataCopy", -2);

    deleter(buffer, RBuffer);
    deallocator(temp);
    return 0;
}

class(RListFinder)
    REnumerateDelegate master;

endOf(RListFinder)

rbool checkObjectRListFinder(pointer data, size_t index) {
    if(index == 2) {
        return no;
    }
    return yes;
}

void printfInt(pointer ptr) {
    printf("%qu\n", (unsigned long long int) ptr);
}

int RListTest(void) {
    size_t iterator;
    RListFinder finder;
    // link virtual
    finder.master.virtualCheckObject = checkObjectRListFinder;

    RList *list = constructorOfRList(nil);
    list->printerDelegate = printfInt;

    forAll(iterator, 20) {
        $(list, m(addHead, RList)), (pointer) iterator);
    }
    RAY_TEST(list->count != 20, "RList. Bad add head.", -1);

    RList* result = $(list, m(subList, RList)), makeRRange(5, 5));
    RAY_TEST(!result, "RList. Bad sublist.", -2);
    RAY_TEST(result->count != 5, "RList. Bad sublist count.", -3);

    RFindResult founded = $(result, m(enumerate, RList)), &finder.master, yes);

    RAY_TEST(!founded.object, "RList. Bad founded object.", -4);
    RAY_TEST(founded.index == list->count, "RList. Bad finded index.", -5);

    deleter(result, RList);

    $(list, m(deleteObjects, RList)), makeRRange(10, 10));
    RAY_TEST(list->count != 10, "RList. Bad count on delete.", -6);

    RArray *array = $(list, m(toRArray, RList)));
    RAY_TEST(!array, "RList. Bad array from list", -7);
    RAY_TEST(list->count != array->count && array->count != 10, "RList. Bad count of array from list.", -8);

    deleter(array, RArray);
    deleter(list, RList);
    return 0;
}

#define TEST_COUNT 10

RArray *arrayTest;

pointer func1(pointer arg) {
    enablePool(RPool);
    int i;
    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RS((char*)arg));
    }
    return 0;
}

int RThreadTest(void) {
    arrayTest = makeRArray();
    int i;
    arrayTest->destructorDelegate = free;
    RThread *thread1 = $(nil, c(RThread)), nil, func1, "1 thread");

    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RS("main"));
    }

    $(thread1, m(join, RThread)));
    deallocator(thread1);

    RAY_TEST(arrayTest->count != 2 * TEST_COUNT, "RThread bad array count.", -1);
    deleter(arrayTest, RArray);

    return 0;
}

void ComplexTest() {
    srand((unsigned int) time(nil));
    if(
           !RDynamicArrayTest()
        && !RListTest()
        && !RClassNamePairTest()
        && !RClassTableTest()
        && !RDictionaryTest()
        && !StringArrayTest()
        && !StringDictionaryTest()
        && !RByteArrayTest()
        && !RBufferTest()
        #ifndef __WIN32
            && !RThreadTest() // fixme in progress
        #endif
    ) {
        RPrintf("All tests passed successfully\n");
    } else {
        RError("TESTS ERROR!", nil);
    }
}