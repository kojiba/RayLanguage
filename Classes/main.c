/**
* main.c
* Ray additions test.
* C additions like dynamic array.
* Author Kucheruavyu Ilya (kojiba@ro.ru)
*/

#include "RayFoundation/RayFoundation.h"
#include "RayFoundation/RDynamicArray/RDynamicArray.h"
#include <stdio.h>
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
    RDynamicArray *dynamicArray = $(NULL, c(RDynamicArray)), NULL);

    dynamicArray->printer = printString;
    dynamicArray->destructor = free;

    for (int i = 0; i < 20; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        $(dynamicArray, m(addObject, RDynamicArray)), a);
    }

    $(dynamicArray, p(RDynamicArray)));
//        $(dynamicArray, m(bubbleSortWithDelegate, RDynamicArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
    $(dynamicArray, m(sort, RDynamicArray)));

    $(dynamicArray, p(RDynamicArray)));

    $(dynamicArray, d(RDynamicArray)));
    $deallocator(dynamicArray);
//    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/*

void RClassTableTest(void){
    printRCTS;
    RegisterClassOnce("Luke");
    printRCTS;

    RegisterClassOnce("Dart");
    printRCTS;

    RegisterClassOnce("Leia");
    printRCTS;

    RegisterClassOnce("Han Solo");
    printRCTS;


    $($master(RCTSingleton,RDynamicArray), m(flush, RDynamicArray)));
    $(RCTSingleton, p(RClassTable)) );

    d(RClassTable)(RCTSingleton);
    $deallocator(RCTSingleton);
}

void RClassNamePairTest(void){
    RClassNamePair *pair = $(NULL, c(RClassNamePair)) );

    pair->className = toString(RClassNamePair);
    pair->idForClassName = 4;
    $(pair, p(RClassNamePair)) );

    $(pair, d(RClassNamePair)) );
    $deallocator(pair);
}

void printString(char *src){
    printf("%s \n",src);
}


void RDynamicArrayTest(void){
    for(int j = 0; j < 10; ++j){
        RDynamicArray *dynamicArray = $(NULL, c(RDynamicArray)), NULL);

        dynamicArray->printer = printString;
        dynamicArray->destructor = free;

        for(int i = 0; i < 17; ++i) {
            char* a = malloc(sizeof(char) * 8);
            memmove(a, "Hello  ", sizeof("Hello  "));
            a[6] = (char)( i % 10 + 48);
            $(dynamicArray, m(addObject, RDynamicArray)), a);
        }

        $(dynamicArray, p(RDynamicArray)) );
//        $(dynamicArray, m(bubbleSortWithDelegate, RDynamicArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
        $(dynamicArray, m(sort, RDynamicArray)) );

        $(dynamicArray, p(RDynamicArray)) );

        $(dynamicArray, d(RDynamicArray)) );
        $deallocator(dynamicArray);
    }
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
    $deallocator(ob1);

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