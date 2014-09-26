/**
 * main.c
 * Ray additions test.
 * C additions like dynamic array.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RArray/RArray.h"
#include "RayFoundation/RClassTable/RClassTable.h"
#include "RayFoundation/RDictionary/RDictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

byte checkString(uint64_t first, uint64_t second) {
    if(first == second){
        return equals;
    }
}

void printString(char *src) {
    printf("%s \n", src);
}

byte stringSorter(char *first, char *second) {
    if (first != second) {
        if (first[6] > second[6]) {
            return swap_objects;
        }
    }
    return 0;
}

int main(int argc, const char *argv[]) {
    startRClock();
//    for(int j = 0; j < 10; ++j){
    RArray *dynamicArray = makeRArray();


    dynamicArray->printerDelegate = printString;
    dynamicArray->destructorDelegate = free;

    for (int i = 0; i < 100000; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToRA(dynamicArray, a);
    }
//    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RArray)), findString);
//    printf("%p\n", findedObject);

//    printRA(dynamicArray);
//        $(dynamicArray, m(bubbleSortWithDelegate, RArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RArray)), 0, dynamicArray->count - 1, stringSorter);
    sortRA(dynamicArray);
//    printRA(dynamicArray);

    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), 895, 10);
    printRA(sub);

    sizeToFitRA(sub);
    printRA(sub);

    deleteRA(dynamicArray);
    deallocator(sub);
//    }

    stopRClock();
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/*

void RDictionaryTest(void){
    RDictionary *dictionary = makeRDictionary();
    uint64_t iterator;
    fromStartForAll(iterator, 1, 20){
        uint64_t value = iterator;
        uint64_t key = iterator;
        $(dictionary, m(setObjectForKey, RDictionary)), value, key);
    }

    printRD(dictionary);
    deleteRD(dictionary);
}

void RClassTableTest(void){
    startRClock();

    registerClassOnce("Luke");
    registerClassOnce("Dart");
    registerClassOnce("Leia");
    registerClassOnce("Han Solo");
    // try once more, but here is only one record
    uint64_t iterator;
    forAll(iterator, 1000000) {
        registerClassOnce("Leia");
        registerClassOnce("Dart");
        registerClassOnce("Luke");
    }

    printRCTS;

    char *checkName = "Han Solo";
    RPrintf("Identifier of %s is - %qu \n", checkName, registerClassOnce(checkName));

    stopRClock();
}

void RClassNamePairTest(void){
    RClassNamePair *pair = $(NULL, c(RClassNamePair)) );

    pair->className = toString(RClassNamePair);
    pair->idForClassName = 4;
    $(pair, p(RClassNamePair)) );

    $(pair, d(RClassNamePair)) );
    deallocator(pair);
}

void printString(char *src){
    printf("%s \n",src);
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
    // and subarray will consist two last elements like null
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