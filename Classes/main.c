/**
 * main.c
 * Ray additions test.
 * C additions like dynamic array.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RDynamicArray/RDynamicArray.h"
#include "RayFoundation/RClassTable/RClassTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

byte findString(char *string) {
    return object_founded;
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
//    for(int j = 0; j < 10; ++j){
    RDynamicArray *dynamicArray = makeRDArray();


    dynamicArray->printerDelegate = printString;
    dynamicArray->destructorDelegate = free;

    for (int i = 0; i < 1000; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToRDA(dynamicArray, a);
    }
//    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RDynamicArray)), findString);
//    printf("%p\n", findedObject);

    printRDA(dynamicArray);
//        $(dynamicArray, m(bubbleSortWithDelegate, RDynamicArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
    sortRDA(dynamicArray);
    printRDA(dynamicArray);

    RDynamicArray *sub = $(dynamicArray, m(getSubarray, RDynamicArray)), 895, 10);
    printRDA(sub);

    sizeToFitRDA(sub);
    printRDA(sub);

    deleteRDA(dynamicArray);
    deallocator(sub);
//    }
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
    RDynamicArray *dynamicArray = makeRDArray();


    dynamicArray->printerDelegate = printString;
    dynamicArray->destructorDelegate = free;

    for (int i = 0; i < 1000; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToRDA(dynamicArray, a);
    }
//    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RDynamicArray)), findString);
//    printf("%p\n", findedObject);

    printRDA(dynamicArray);
//        $(dynamicArray, m(bubbleSortWithDelegate, RDynamicArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
    sortRDA(dynamicArray);
    printRDA(dynamicArray);

    RDynamicArray *sub = $(dynamicArray, m(getSubarray, RDynamicArray)), 895, 10);
    printRDA(sub);

    sizeToFitRDA(sub);
    printRDA(sub);

    deleteRDA(dynamicArray);
    deallocator(sub);
//    }
}


void RayFoundationTest(void) {

    printf("Heap: \n\n");
    // Work with pointers
    MyClass *ob1;
    ob1 = $(NULL, c(MyClass)) );
    $(ob1, m(MyMethod, MyClass)), 1);
    $(ob1, m(MyMethod, MyClass)), 26062000);

    $(NULL, sm(MyStaticMethod, MyClass)), 0.451);

    $(ob1, p(MyClass)));
    $(ob1, d(MyClass)));
    // additional pointer cleanup
    deallocator(ob1);

    // Work with stack
    printf("\nStack: \n\n");
    {
        MyClass ob2 = *$(NULL, c(MyClass)) );
        $(&ob2, m(MyMethod, MyClass)), 1);
        $(&ob2, m(MyMethod, MyClass)), 23082011);

        $(NULL, sm(MyStaticMethod, MyClass)), 0.451);

        $(&ob2, p(MyClass)) );
        $(&ob2, d(MyClass)) );
    }

    {
        SimpleDisciple ob3 = *$(NULL, c(SimpleDisciple)) );

        $(&ob3, p(SimpleDisciple)) );
        $(&ob3, d(SimpleDisciple)) );
    }

}
*/