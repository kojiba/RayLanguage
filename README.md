RayLanguage
===========

firstly:
Ray additions to C language defines some C-based syntax, 
that makes object-oriented life easier for C developers.
All based on defines. Here you can find dynamic array for C.

secondly:
There is BEST dynamic array for C language.
There is classTable, that can help you to fast audite C code.

Work with RDynamicArray:

```C
#include "RDynamicArray.h"

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
    // create your dynamic array
    RDynamicArray *dynamicArray = makeRDArray();

    dynamicArray->printerDelegate = printString;
    dynamicArray->destructorDelegate = free;

    for (int i = 0; i < 10; ++i) {
        char *a = malloc(sizeof(char) * 8);
        memmove(a, "Hello  ", sizeof("Hello  "));
        a[6] = (char) (i % 10 + 48);
        addObjectToArray(dynamicArray, a);
    }
    // find object in array
    pointer findedObject = $(dynamicArray, m(findObjectWithDelegate, RDynamicArray)), findString);
    printf("%p\n", findedObject);
    
    // print your array
    printArray(dynamicArray);
    
    // sort with delegate
    $(dynamicArray, m(quickSortWithDelegate, RDynamicArray)), 0, dynamicArray->count - 1, stringSorter);
    
    // built-in sort
    sortArray(dynamicArray);
    printArray(dynamicArray);
    
    // get sub-arrays, there will be two errorrs logged in console, 
    // and subarray will consist thwo last elements like null
    RDynamicArray *sub = $(dynamicArray, m(getSubarray, RDynamicArray)), 1, 11);
    printArray(sub);
    
    // fast delete objects
    $(dynamicArray, m(fastDeleteObjectAtIndexIn, RDynamicArray)), 9);
    printArray(dynamicArray);
    
    // make your array size-to-fit (without free space)
    sizeToFit(dynamicArray);
    printArray(dynamicArray);
    
    // delete your array
    deleteArray(dynamicArray);
    deallocator(sub);

    return 0;
}
```

Work with RClassTable:

```C
#include "RClassTable.h"

void RClassTableTest(void){
    // register class name once, and get identifier in result
    registerClassOnce("Luke");
    // print our class table
    printRCTS;

    registerClassOnce("Dart");
    printRCTS;

    registerClassOnce("Leia");
    printRCTS;

    registerClassOnce("Han Solo");
    
    // flush your class table
    flushRCTS;
    printRCTS;
    
    // delete your class table
    releaseRCTS;
}
```
