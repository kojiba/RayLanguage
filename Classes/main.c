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

byte findString(char *string) {
    return equals;
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
////    for(int j = 0; j < 10; ++j){
//    RArray *dynamicArray = makeRArray();
//
//
//    dynamicArray->printerDelegate = printString;
//    dynamicArray->destructorDelegate = free;
//
//    for (int i = 0; i < 1000; ++i) {
//        char *a = malloc(sizeof(char) * 8);
//        memmove(a, "Hello  ", sizeof("Hello  "));
//        a[6] = (char) (i % 10 + 48);
//        addObjectToRA(dynamicArray, a);
//    }
////    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RArray)), findString);
////    printf("%p\n", findedObject);
//
//    printRA(dynamicArray);
////        $(dynamicArray, m(bubbleSortWithDelegate, RArray)), stringSorter);
////        $(dynamicArray, m(quickSortWithDelegate, RArray)), 0, dynamicArray->count - 1, stringSorter);
//    sortRA(dynamicArray);
//    printRA(dynamicArray);
//
//    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), 895, 10);
//    printRA(sub);
//
//    sizeToFitRA(sub);
//    printRA(sub);
//
//    deleteRA(dynamicArray);
//    deallocator(sub);
////    }

//    registerClassOnce("Luke");
//    registerClassOnce("Dart");
//    registerClassOnce("Leia");
//    registerClassOnce("Han Solo");
//    printRCTS;
//
//    RCString *hello = RS("Hello world\n");
//    printRString(hello);
//
//    char *checkName = "Han Solo";
//    RPrintf("Identifier of %s is - %qu \n", checkName, getIdentifierByName(checkName));

    RDictionary *dictionary = makeRDictionary();
    inDictionary(dictionary) setObject(0x21) forKey(0x50);
    inDictionary(dictionary) setObject(0x22) forKey(0x50);
    printRD(dictionary);
    deleteRD(dictionary);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/*

void RClassTableTest(void){
    registerClassOnce("Luke");
    registerClassOnce("Dart");
    registerClassOnce("Leia");
    registerClassOnce("Han Solo");
    printRCTS;

    RCString *hello = RS("Hello world\n");
    printRString(hello);

    char *checkName = "Han Solo";
    RPrintf("Identifier of %s is - %qu \n", checkName, getIdentifierByName(checkName));
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