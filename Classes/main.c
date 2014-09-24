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

#define linkMethodToImplementation(methodName) instance->methodName = splitUp(methodName, Implementation);
#define overrideMethod(providerClassName, methodNameOld, methodNameNew) instance->splitUp(master, providerClassName)->methodNameOld = instance->methodNameNew;

struct Hello;

typedef struct HelloSingleton {
    void (*destructorOfHello)(struct Hello *object);

} HelloSingleton;
HelloSingleton* singletonOfHello(void);


typedef struct Hello {
    uint64_t classId;
    HelloSingleton *singleton;
    int a;
} Hello;


Hello* ContstructorFuncImplementation(Hello *object) {
    object = allocator(Hello);
    if(object) {
        object->classId = registerClassOnce("Hello");
        object->singleton = singletonOfHello();
        object->a = 0;
    }
    return object;
}

void destructorOfHelloImplementation(struct Hello *object){
    object->classId = 0;
    object->singleton = 0;
    object->a = 0;
}

HelloSingleton* singletonOfHello(void) {
    static HelloSingleton *instance;
    if(instance == NULL) {
        instance = allocator(HelloSingleton);
        // adds virtual methods
        linkMethodToImplementation(destructorOfHello);
    }
    return instance;
}
//------------------------------------------------------------------------------------------------------
struct HelloDisciple;

typedef struct HelloDiscipleSingleton {
    HelloSingleton *masterHello;
    void (*destructorOfHelloDisciple)(struct HelloDisciple *object);
} HelloDiscipleSingleton;
HelloDiscipleSingleton* singletonOfHelloDisciple(void);

typedef struct HelloDisciple {
    uint64_t classId;
    Hello* master;
    HelloDiscipleSingleton *singleton;
    int b;
} HelloDisciple;

HelloDisciple* constructorOfHelloDiscipleImplementation(void) {
    HelloDisciple *object = allocator(HelloDisciple);
    if(object) {
        object->master = ContstructorFuncImplementation(object->master);
        object->classId = registerClassOnce("HelloDisciple");
        object->singleton = singletonOfHelloDisciple();
    }
    return object;
}

void destructorOfHelloDiscipleImplementation(HelloDisciple *object){
    object->b = 0;
    object->singleton = 0;
    object->classId = 0;
    destructorOfHelloImplementation(object->master);
}

HelloDiscipleSingleton* singletonOfHelloDisciple(void){
    static HelloDiscipleSingleton *instance;
    if(instance == NULL) {
        instance = allocator(HelloDiscipleSingleton);
        instance->masterHello = singletonOfHello();

        // override
        overrideMethod(Hello, destructorOfHello, destructorOfHelloDisciple);
        linkMethodToImplementation(destructorOfHelloDisciple);
    }
    return instance;
}

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
//    RDynamicArray *dynamicArray = makeRDArray();
//
//
//    dynamicArray->printerDelegate = printString;
//    dynamicArray->destructorDelegate = free;
//
//    for (int i = 0; i < 10; ++i) {
//        char *a = malloc(sizeof(char) * 8);
//        memmove(a, "Hello  ", sizeof("Hello  "));
//        a[6] = (char) (i % 10 + 48);
//        addObjectToArray(dynamicArray, a);
//    }
//    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RDynamicArray)), findString);
//    printf("%p\n", findedObject);
//
//    printArray(dynamicArray);
////        $(dynamicArray, m(bubbleSortWithDelegate, RDynamicArray)), stringSorter);
////        $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
//    sortArray(dynamicArray);
//    printArray(dynamicArray);
//
//    RDynamicArray *sub = $(dynamicArray, m(getSubarray, RDynamicArray)), 1, 11);
//    printArray(sub);
//
//    $(dynamicArray, m(fastDeleteObjectAtIndexIn, RDynamicArray)), 9);
//    printArray(dynamicArray);
//    sizeToFit(dynamicArray);
//    printArray(dynamicArray);
//
//    deleteArray(dynamicArray);
//    deallocator(sub);
    registerClassOnce("Luke");
    registerClassOnce("Dart");
    registerClassOnce("Leia");
    registerClassOnce("Han Solo");
    printRCTS;

    char *checkName = "Han Solo";
    RPrintf("Identifier of %s is - %qu \n", checkName, getIdentifierByName(checkName));
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

    RPrintf("Identifier of Han Solo is - %qu \n", getIdentifierByName("Han Solo"));

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