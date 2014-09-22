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

// working defines
//#define _TOSTRING(x) #x
//#define toString(x) _TOSTRING(x)              // gets c-string from define or some code
//
//#define _splitUp(one, two) one##two
//#define splitUp(one, two) _splitUp(one, two)
//
//#define className
//
//#define class(ClassName) typedef struct ClassName {
//
//#define endOfClass(ClassName) } ClassName; \
//                               className splitUp(className, BaseSingleton);
//
//#define method(retValue, methodName) retValue (*splitUp(methodName,className))(struct className *object
//#define implementation(retValue, methodName) retValue splitUp(splitUp(methodName, className), Implementation)(className *object
//#define endImplementation(retValue, methodName)  /*(*splitUp(className,BaseSingleton).splitUp(methodName,className)) = &splitUp(splitUp(methodName, className), Implementation);*/ }
//
//
//#define className Hello
//class(Hello)
//    int b;
//    method(void, getMe), int arg);
//endOfClass(Hello);
//
//implementation(void, getMe), int arg) {
//    printf("%d",arg);
//endImplementation(void, getMe)

int main(int argc, const char *argv[]) {
//    for(int j = 0; j < 10; ++j){
    RDynamicArray *dynamicArray = makeRDArray();


    dynamicArray->printerDelegate = printString;
    dynamicArray->destructorDelegate = free;

    for (int i = 0; i < 1000; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToArray(dynamicArray, a);
    }
//    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RDynamicArray)), findString);
//    printf("%p\n", findedObject);

    printArray(dynamicArray);
//        $(dynamicArray, m(bubbleSortWithDelegate, RDynamicArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
    sortArray(dynamicArray);
    printArray(dynamicArray);

    RDynamicArray *sub = $(dynamicArray, m(getSubarray, RDynamicArray)), 895, 10);
    printArray(sub);

    deleteArray(dynamicArray);
    deallocator(sub);
//    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/*

void RClassTableTest(void){
    registerClassOnce("Luke");
    printRCTS;

    registerClassOnce("Dart");
    printRCTS;

    registerClassOnce("Leia");
    printRCTS;

    registerClassOnce("Han Solo");

    flushRCTS;
    printRCTS;

    releaseRCTS;
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
    RDynamicArray *dynamicArray = $(NULL, c(RDynamicArray)), NULL);

    dynamicArray->printer = printString;
    dynamicArray->destructor = free;

    for (int i = 0; i < 10000; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        $(dynamicArray, m(addObject, RDynamicArray)), a);
    }

//    $(dynamicArray, p(RDynamicArray)));
//        $(dynamicArray, m(bubbleSortWithDelegate, RDynamicArray)), stringSorter);
//        $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
    $(dynamicArray, m(sort, RDynamicArray)));

    $(dynamicArray, p(RDynamicArray)));

    $(dynamicArray, d(RDynamicArray)));
    deallocator(dynamicArray);
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