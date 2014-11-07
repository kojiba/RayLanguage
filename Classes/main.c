/**
 * main.c
 * Ray additions test.
 * C additions like dynamic array.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RSystem.h"

//void*   (*RMallocPtr)(size_t size) = RTrueMalloc;
//void    (*RFreePtr)  (void*  ptr) = RTrueFree;

pointer mySandBoxAlloc(size_t size);

RSandBox* mySingleton(void) {
    static RSandBox *instance = nullPtr;
    if(instance == nullPtr) {
        instance = $(NULL, c(RSandBox)), 4096, 100, RTrueMalloc, RTrueFree);
        instance->innerMallocPtr = mySandBoxAlloc;
//        instance->allocationMode = RSandBoxAllocationModeStandart;
    }
    return instance;
}

pointer mySandBoxAlloc(size_t size) {
    RPrintf("RSandBox malloc in - %p\n", mySingleton());
    return $(mySingleton(), m(malloc, RSandBox)), size);
}

pointer emptyRealloc(pointer ptr, size_t size) {
    RPrintf("--- Realloc for %p, new size : %qu (bytes)\n", ptr, size);
    size_t oldSize = $(mySingleton(), m(sizeForPointer, RSandBox)), ptr);
    if(oldSize == 0) {
        RError("Bad pointer", mySingleton());
    }
    pointer newBuffer = mySandBoxAlloc(size);
    RMemMove(newBuffer, ptr, oldSize);
    return newBuffer;
}

int main(int argc, const char *argv[]) {
    initPointers();
    RReallocPtr = emptyRealloc;
    RPrintCurrentSystem();
    RPrintf("Sizeof pointer - %qu\n", sizeof(pointer));
    const size_t size = 70;
    size_t iterator;

    enableSandBoxMalloc(mySandBoxAlloc);  // enable our sandbox
    RArray *array = $(nullPtr, c(RArray)), nullPtr); // leaks
    forAll(iterator, size) {
        addObjectToRA(array, iterator);
    }
    printRA(array);
    $(array, m(sizeToFit, RArray)));
    disableSandBoxMalloc();         // disable sandbox


    $(mySingleton(), p(RSandBox)) );
    $(mySingleton(), d(RSandBox)) );

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/*

void StringDictionaryTest() {
    RStringDictionary *stringDictionary = makeRStringDictionary();
    $(stringDictionary, m(setObjectForKey, RStringDictionary)), RS("Value"),   RSC("Key"));
    $(stringDictionary, m(setObjectForKey, RStringDictionary)), RS("Value-2"), RSC("Key-2"));
    $(stringDictionary, p(RStringDictionary)) );

    deleteRStringDictionary(stringDictionary);
}

void BRFinterpreter() {
    char *code = RAlloc(1000);
    RPrintf("Input some brainfuck code:\n");
    RScanf("%1000s", code);
    if(code != nullPtr) {
        RVirtualFunction *function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)),
                RS(code));

        executeRay(function);

        $(function, d(RVirtualFunction)) );
        deallocator(function);
        deallocator(code);
        deleteRVM();
    }
}

void cycleTest() {
    // cycle test
    // rasm program
    RVirtualFunction *function = $(function, c(RVirtualFunction)) );
    function->name = RS("If test");
    master(function, RByteArray) = makeRByteArray(20);
    master(function, RByteArray)->array[0] = r_increment;
    master(function, RByteArray)->array[1] = r_increment;
    master(function, RByteArray)->array[2] = r_increment;
    master(function, RByteArray)->array[3] = r_if;
    master(function, RByteArray)->array[4] = r_goto_address;
    master(function, RByteArray)->array[5] = 8;
    master(function, RByteArray)->array[6] = r_decrement;
    master(function, RByteArray)->array[7] = r_print_char;
    master(function, RByteArray)->array[8] = r_if_not;
    master(function, RByteArray)->array[9] = r_goto_address;
    master(function, RByteArray)->array[10] = 3;
    master(function, RByteArray)->array[11] = r_end;
}

void RVM_basicsTest() {
//    // brainfuck hard(with [, ]) hello world on RVM
//    RVirtualFunction *function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)),
//            RS(" Hard Hello world : ++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++\n"
//                                  " .>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.\n"
//                                  " ------.--------.>+.>."));
    // brainfuck ezy hello world on RVM
    RVirtualFunction *function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)),
            RS(" My brainfuck hello world : +++++++++++++++++++++++++++++++++++++++++++++\n"
                                          " +++++++++++++++++++++++++++.+++++++++++++++++\n"
                                          " ++++++++++++.+++++++..+++.-------------------\n"
                                          " ---------------------------------------------\n"
                                          " ---------------.+++++++++++++++++++++++++++++\n"
                                          " ++++++++++++++++++++++++++.++++++++++++++++++\n"
                                          " ++++++.+++.------.--------.------------------\n"
                                          " ---------------------------------------------\n"
                                          " ----.-----------------------."));

    executeRay(function);

    $(function, d(RVirtualFunction)) );
    deallocator(function);
    deleteRVM();

    return 0;
}

void StringDictionaryTest() {
    // some key constant
    RCString *key = RS("Veider");

    // create dictionary
    RStringDictionary *dictionary = $(nullPtr, c(RStringDictionary)) );

    // fill dictionary with some object-keys,
    // use RSC, cause we need copies of constant,
    // also use m(copy, RCString)
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Value"), RSC("Key"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Leia"), RSC("Han Solo"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Luke"), $(key, m(copy, RCString))) );

    // try once more, to check is it one-time-adding
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Value"), RSC("Key"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Leia"), RSC("Han Solo"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Luke"), $(key, m(copy, RCString))) );

    // print
    $(dictionary, p(RStringDictionary)) );

    // find some object for key
    RCString *object = $(dictionary, m(getObjectForKey, RStringDictionary)), key);
    if(object != nullPtr) {
        RPrintf("Found something for key : %s is value: %s\n", key->baseString, object->baseString);
    }

    // destructs, and delete pointer
    $(dictionary, d(RStringDictionary)) );
    deallocator(dictionary);
}

void StringArrayTest() {
    RArray *stringArray = makeRArray();
    stringArray->printerDelegate = p(RCString);
    stringArray->destructorDelegate = d(RCString);

    for(unsigned i = 0; i < 10; ++i) {
        addObjectToRA(stringArray, randomRCString());
    }
    printRA(stringArray);

    $(stringArray, m(deleteObjects, RArray)), makeRRange(5, 4));

    printRA(stringArray);
}

void RATimingTest() {
    initRClock();
    double average = 0;
    size_t count = 173137;

    for(double multiplier = 1.5; multiplier < 40; multiplier += 0.5) {

        for (size_t i = 0; i < 30; ++i) {
            RArray *stringArray = makeRArray();
            stringArray->destructorDelegate = RFree;
            stringArray->sizeMultiplier = multiplier;

            for (size_t j = 0; j < count; ++j) {
                char *a = (char *) malloc(sizeof(char) * 8);
                memmove(a, "Hello  ", sizeof("Hello  "));
                a[6] = (char) (j % 10 + 48);
                addObjectToRA(stringArray, a);
            }
            deleteRA(stringArray);
            tickRClock();
            average = average / 2.f + diff / 2.0f;
        }

        average = average * 1000 / CLOCKS_PER_SEC;
        RPrintf("%f\n", average);
        average = 0;
    }
}

void RDictionaryTest(void){
    RDictionary *dictionary = makeRDictionary();
    size_t iterator;
    fromStartForAll(iterator, 1, 20){
        size_t value = iterator;
        size_t key = iterator;
        $(dictionary, m(setObjectForKey, RDictionary)), value, key);
    }

    printRD(dictionary);
    deleteRD(dictionary);
}

void RClassTableTest(void){
    initRClock();

    registerClassOnce("Luke");
    registerClassOnce("Dart");
    registerClassOnce("Leia");
    registerClassOnce("Han Solo");
    // try once more, but here is only one record
    size_t iterator;
    forAll(iterator, 1000000) {
        registerClassOnce("Leia");
        registerClassOnce("Dart");
        registerClassOnce("Luke");
    }

    printRCTS;

    char *checkName = "Han Solo";
    RPrintf("Identifier of %s is - %qu \n", checkName, registerClassOnce(checkName));

    tickRClock();
}

void RClassNamePairTest(void){
    RClassNamePair *pair = $(nullPtr, c(RClassNamePair)) );

    pair->className = toString(RClassNamePair);
    pair->idForClassName = 4;
    $(pair, p(RClassNamePair)) );

    $(pair, d(RClassNamePair)) );
    deallocator(pair);
}

void RDynamicArrayTest(void){
//    for(int j = 0; j < 10; ++j){
    RArray *dynamicArray = makeRArray();


    dynamicArray->printerDelegate = printString;
    dynamicArray->destructorDelegate = free;

    for (int i = 0; i < 1000; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToRA(dynamicArray, a);
    }
//    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RArray)), findString);
//    printf("%p\n", findedObject);

    printRA(dynamicArray);
//        $(dynamicArray, m(bubbleSortWithDelegate, RArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RArray)), 0, dynamicArray->count - 1, stringSorter);
    sortRA(dynamicArray);
    printRA(dynamicArray);

    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), 895, 10);
    printRA(sub);

    sizeToFitRA(sub);
    printRA(sub);

    deleteRA(dynamicArray);
    deallocator(sub);
//    }
}

void RDynamicArrayTest2(void){
    // create your dynamic array
    RArray *dynamicArray = makeRArray();

    dynamicArray->printerDelegate = printString;
    dynamicArray->destructorDelegate = free;

    for (int i = 0; i < 10; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToRA(dynamicArray, a);
    }

    // print your array
    printRA(dynamicArray);

    // built-in sort
    sortRA(dynamicArray);
    printRA(dynamicArray);

    // sort with delegate
    $(dynamicArray, m(quickSortWithDelegate, RArray)), 0, dynamicArray->count - 1, stringSorter);
    printRA(dynamicArray);

    // get sub-arrays, there will be two errorrs logged in console,
    // and subarray will consist two last elements like nullPtr
    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), 1, 11);
    printRA(sub);

    // fast delete objects
    $(dynamicArray, m(fastDeleteObjectAtIndexIn, RArray)), 9);
    printRA(dynamicArray);

    // make your array size-to-fit (without free space)
    sizeToFitRA(dynamicArray);
    printRA(dynamicArray);

    // delete your array
    deleteRA(dynamicArray);
    deallocator(sub);
}
*/