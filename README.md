RayLanguage
===========

firstly:
Ray additions to C language defines some C-based syntax, 
that makes object-oriented life easier for C developers.
All based on defines. Here you can find dynamic array for C.

secondly:
There is BEST dynamic array for C language.
There is classTable, that can help you to fast audite C code.

Work with RArray:

```C
#include "RayFoundation/RArray/RArray.h"

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
    
    // try once more, but here is only one record
    registerClassOnce("Leia");
    registerClassOnce("Dart");
    registerClassOnce("Luke");
    registerClassOnce("Han Solo");
    printRCTS;
    // get Identifier of Han Solo
    char *checkName = "Han Solo";
    RPrintf("Identifier of %s is - %qu \n", checkName, registerClassOnce(checkName));
    
    // flush your class table
    flushRCTS;
    printRCTS;
    
    // delete your class table
    releaseRCTS;
}
```

You can simply use it in Yours C++ projects:

```C
#include <iostream>

extern "C" {
    #include "RArray.h"
}

using namespace std;

class MyClass{
protected:
    double x;
    double y;
public:
    MyClass(){

    }
    ~MyClass(){

    }
    void Print(){
        cout << "MyClass obj - " << this << endl;
    }
};

void deleter(pointer src) {
    delete (MyClass*)src;
}

void Printer(pointer src){
    ((MyClass*)src)->Print();
}

int main(int argc, const char *argv[]) {

    RArray *helloArray = makeRArray();

    helloArray->destructorDelegate = deleter;
    helloArray->printerDelegate = Printer;

    for(unsigned i = 0; i < 100000; ++i) {
        MyClass *a = new MyClass;
        addObjectRArray(helloArray, a);
    }

    $(helloArray, p(RArray)));
    deleteRA(helloArray);
    return 0;
}
```
