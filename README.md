RayLanguage
===========

firstly:
Ray additions to C language defines some C-based syntax, 
that makes object-oriented life easier for C developers.
All based on defines.

Containers:
1.  Array (dynamicly self-longs, store pointers to objects)
2.  Dictionary (NSDictionary, std::map analog, self-longs)
3.  Buffer (self-longs, sore full copy of objects)
4.  StringDictionary (Dictionary with keys like strings)
5.  ClassTable (for store pairs of number and string once)

Strings:
1. Wide range of c-string processing operations (RCString)
2. Replacings
3. Find of substring or symbol
4. delete characters/substrings
5. delete of duplicatings characters/substring
6. compares
7. read from file/ apend to file
8. etc...

Memory operations:
1. Self-code easy sandboxing and testing with logging.
2. Work with byte buffers
3. Work with memory chunks

Some test projects based on RayFoundation:
1. Simple VM with Brainfuck compiler
2. Simple lang interpreter (unfinished)

Work with RArray:

```C
#include "RayFoundation/RArray/RArray.h"

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
    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), makeRRange(1, 11));
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
    
    //--------------------------------------------------
    
    // create c-string array
    RArray *stringArray = makeRArray();
    
    // set-up delegates
    stringArray->printerDelegate = p(RCString);
    stringArray->destructorDelegate = d(RCString);

    for(unsigned i = 0; i < 10; ++i) {
        // genering simly-rand() strings
        addObjectToRA(stringArray, randomRCString());
    }
    printRA(stringArray);

    // delete subArray in range [5...9]
    $(stringArray, m(deleteObjects, RArray)), makeRRange(5, 4));

    printRA(stringArray);
}
```

BrainFuck samples:

```C
#include "RayFoundation/RayFoundation.h"
#include "RVirtualMachine/RVirtualFunction/RVirtualFunction.h"
#include "RVirtualMachine/RVirtualMachine/RVirtualMachine.h"
#include "RVirtualMachine/RVirtualCompiler.h"

int main(int argc, const char *argv[]) {
    // ezy brainfuck hello world
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

    // execute of byte-code on RVM singleton
    executeRay(function);
    $(function, d(RVirtualFunction)) );

    // brainfuck compiler multiple-cycles test
    function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)),
            RS(" Cycles : +++ [ > +++ [.-] <-]")); // prints '03 02 01' 3-times
    executeRay(function);
    $(function, d(RVirtualFunction)) );

    // brainfuck hard(with [, ]) hello world on RVM
    function = $(RVC, m(createFunctionFromBrainFuckSourceCode, RVirtualCompiler)),
            RS(" Hard Hello world : ++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++\n"
                                  " .>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.\n"
                                  " ------.--------.>+.>."));

    // rasm byte-code print in words
    $(function, p(RVirtualFunction)) );

    executeRay(function);
    $(function, d(RVirtualFunction)) );
    
    // final function delete
    deallocator(function);

    // RVM singleton cleanup
    deleteRVM();
    return 0;
}
```

Work with RClassTable:

```C
#include "RayFoundation/RClassTable/RClassTable.h"

void RClassTableTest(void){

    // register class name once, and get identifier in result
    // it's can be used like management tool for creating unique 
    // identifiers for string objects (in our case class names)
    
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
    
    // use not singleton
    RClassTable *table = makeRCTable();

    $(table, m(registerClassWithName, RClassTable)), "Some string");
    $(table, m(registerClassWithName, RClassTable)), "Some string2");
    $(table, m(registerClassWithName, RClassTable)), "Some string3");
    // once more time, to check
    $(table, m(registerClassWithName, RClassTable)), "Some string3");

    $(table, p(RClassTable)) );

    $(table, d(RClassTable)) );
    deallocator(table);
}
```

Use of RStringDictionary:

```C

#include "RayFoundation/RStringDictionary/RStringDictionary.h"

void StringDictionaryTest() {
    // some key constant
    RCString *key = RS("Veider");

    // create dictionary
    RStringDictionary *dictionary = $(NULL, c(RStringDictionary)) );

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
    if(object != NULL) {
        RPrintf("Found something for key : %s is value: %s\n", key->baseString, object->baseString);
    }

    // destructs, and delete pointer
    $(dictionary, d(RStringDictionary)) );
    deallocator(dictionary);
}
```

You can simply use it in Yours C++ projects:

```C++
#include <iostream>

#include "RayFoundation.h"

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

```C
#include "RayFoundation/RFloatingEnum/RFloatingEnum.h"

typedef enum codes {
    first_opcode,
    second_opcode,
    third_opcode,
    fourth_opcode,
    opcode_count
} codes;

int main(int argc, const char *argv[]) {
 createFloatingEnum(codes, rand, first_opcode, opcode_count);
    // cause not seed rand, in my case 282475249 is Bingo
    if(checkValueToKey(codes, 282475249, second_opcode) == equals) {
        RPrintf("Bingo!\n");
    } else {
        RPrintf("Not Bingo =(");
    }
    // print enum
    printEnum(codes);

    // rebase manually
    rebaseEnum(codes);
    printEnum(codes);

    // enables rebasing after all checks
    setFloatingEnum(codes);

    checkValueToKey(codes, 282475249, second_opcode);
    printEnum(codes);
    printEnum(codes);
    checkValueToKey(codes, 282475249, second_opcode);
    printEnum(codes);

    deleteEnum(codes);
    return 0
}
```

