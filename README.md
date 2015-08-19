RayLanguage
===========

**If You have some ideas, or found bugs - create issues here on github or email me.**

firstly:
Ray additions to C language defines some C-based syntax, 
that makes object-oriented life easier for C developers.
All based on defines.  
  
Containers:  
1.  Array, List (dynamicly self-longs, store pointers to objects)  
2.  Dictionary (NSDictionary, std::map analog, self-longs)  
3.  Buffer (self-longs, sore full copy of objects)  
5.  ClassTable (for store pairs of number and string once, may be used like runtime enum etc.)  
  
Strings:  
1. Wide range of c-string processing operations (RCString)  
2. Replacings  
3. Find of substring or symbol  
4. Delete characters/substrings  
5. Delete of duplicatings characters/substring  
6. Compares  
7. Read from file/ apend to file  
8. Tokenization into containeer Array  
9. Base64 encoding/decoding  
10. etc...  
  
Memory operations:  
1. Self-code easy sandboxing and testing with logging.   
2. Self-code management with RAutoPool. (Checking leaks)   
3. Work with byte buffers    
4. Work with memory chunks    

Some test projects based on RayFoundation:  
1. Simple VM with Brainfuck compiler  
2. Simple lang interpreter (unfinished)  

Work with RArray:  

```C
#include <RayFoundation.h>

typedef struct ComplexStruct {
    size_t id;
    RCString *fullName;
} ComplexStruct;

void printComplexStruct(ComplexStruct *some) {
    printf("%p | id: %02lu | fullname : \'%s\' \n", some, some->id, some->fullName->baseString);
}

void complexDeleter(ComplexStruct *some) {
    deleter(some->fullName, RCString);
    free(some);
}

int main(int argc, const char *argv[]) {
    size_t iterator;

    RArray *array = makeRArray(); // create array
    // sets delegates for automatic print and cleanup
    array->printerDelegate = (PrinterDelegate) printComplexStruct;
    array->destructorDelegate = (DestructorDelegate) complexDeleter;

    forAll(iterator, 100) {
        // create struct and add to array
        ComplexStruct *some = malloc(sizeof(ComplexStruct));
        some->id = iterator + 1;
        some->fullName = stringWithFormat("Object # %lu", iterator + 1); // more string processing API in RCString.h
        $(array, m(addObject, RArray)), some);
    }
    // print
    printerOfRArray(array);

    // delete, automatically calls destructor delegate and free array prt
    deleter(array, RArray);

    // some syntaxic sugar see initFromArray, initFromArrayWithSizes, arrayFromArray

    // create array of strings
    RCString *uniqTok = RS("Unique token");
    array = RA(RS("Token 1"), RS("Token 2"), uniqTok, nil); // must be NULL terminated

    array->printerDelegate = (PrinterDelegate) p(RCString);
    array->destructorDelegate = free;

    printerOfRArray(array);

    RCompareDelegate delegate;
    delegate.etaloneObject = uniqTok;
    delegate.virtualCompareMethod = (RCompareFlags (*)(pointer, pointer)) compareWithRCString;

    RFindResult result = findObjectWithDelegateRArray(array, &delegate); // search

    if(result.object != nil) { // object != nil -> found object, place also logged
        printf("Found some object at place %lu!\n", result.index);
    } else {
        printf("Object not found!\n");
    }

    RArray *subarray = getSubarrayRArray(array, makeRRange(1, 2)); // delegates will be copied
    printerOfRArray(subarray);
    subarray->destructorDelegate = nil; // not want to delete strings
    deleter(subarray, RArray);

    deleter(array, RArray); // cleanup strings

    return 0;
}
```

RDictionary use:  

```C
#include <RayFoundation.h>
#include "Tests.h"

int main(int argc, const char *argv[]) {
    enablePool(RPool);
    ComplexTest();

    RDictionary *dict =  dictionaryFromPairs("key", "value",
                                             "key2", "value2",
                                             "key3", "value3",
                                             "key4", "value4",
                                             "key5", "value5",
                                             nil);

    char * string = $(dict, m(getObjectForKey, RDictionary)), "key4");

    printf("Found object for \'key4\' - \'%s\' \n\n", string);

    deleter(dict, RDictionary);

    endRay(); // standart cleanup
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
    endRay(); // standart cleanup
}
```
RAutoPool:  

```C
int main(int argc, const char *argv[]) {
    initPointers();
    enablePool(RPool); // enable pool sinleton
    ComplexTest();     // do something 

    $(RPool, p(RAutoPool)));   // check leaks
    deleter(RPool, RAutoPool); // total cleanup 
    return 0;
    // or use macro endRay -> standart cleanup
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
Some multi-thread tcp-echo server sample on RTCPHandler:  

```C
#include <RayFoundation/RayFoundation.h>

#include "Tests.h"

#define BUFFER_SIZE 1500

pointer exec(RTCPDataStruct *data) {
    char    buffer[BUFFER_SIZE];
    const char    *address = addressToString(&data->socket->address);
    ushort            port = ntohs(data->socket->address.sin_port);
    unsigned currentThread =  (unsigned int) currentThreadIdentifier();
    byte     resultFlag;
    size_t   receivedSize;

    RPrintf("[I] %s:%u connected [tuid : %u]\n", address, port, currentThread);

    $(data->socket, m(sendString, RSocket)), RS("Hello from RServer.\n"));

    resultFlag = $(data->socket, m(receive, RSocket)), buffer, 1000, &receivedSize);
    while(resultFlag != networkConnectionClosedConst) {
        buffer[receivedSize] = 0;
        RPrintf("%s:%u[%u] > %s", address, port, currentThread, buffer);
        $(data->socket, m(send, RSocket)), buffer, receivedSize);
        resultFlag =  $(data->socket, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
    }

    RPrintf("[I] %s:%u disconnected [tuid : %u]\n", address, port, currentThread);

    deleter(data->socket, RSocket);
    data->socket = nil; // for auto-cleaning
    return nil;
}

RTCPHandler  *server   = nil;
RTCPDelegate *delegate = nil;

void startServer(void) {
    server = c(RTCPHandler)(nil);
    if(server != nil) {
        delegate = allocator(delegate);
        if(delegate != nil) {
            delegate->delegateFunction = (RThreadFunction) exec;
            delegate->context          = server;
            $(server,  m(set_delegate, RTCPHandler)), delegate);
            RPrintf("RTCPHandler starting %p\n", server);
            $(server,  m(startOnPort, RTCPHandler)), 4000);
        }
    }
}

int main(int argc, const char *argv[]) {
    rbool closeAll = no;
    byte connectionState;
    char buffer[BUFFER_SIZE];
    const char *address;
    ushort port;
    RSocket *configurator;
    size_t  receivedSize;

    enablePool(RPool);
    ComplexTest(); // lib test

    startServer();

    configurator = openListenerOnPort(4001, 10);
    if(configurator == nil) goto exit;

    while(!closeAll) {
        RSocket *current = $(configurator, m(accept, RSocket)));

        address = addressToString(&current->address);
        port    = ntohs(current->address.sin_port);

        RPrintf("[I] Configurator %s:%u connected\n", address, port);
        connectionState = networkOperationSuccessConst;

        while(connectionState != networkConnectionClosedConst) {
            connectionState = $(current, m(receive, RSocket)), buffer, BUFFER_SIZE, &receivedSize);
            if(connectionState == networkOperationSuccessConst) {
                if(receivedSize > 8) {
                    buffer[receivedSize] = 0;
                    ifMemEqual(buffer, "secretkey", 9) {

                        ifMemEqual(buffer + 10, "shutdown", 8) {
                            $(current, m(sendString, RSocket)), RS("Server will terminate\n"));
                            RPrintf("[I] Will terminate with command from %s:%u\n\n", address, port);

                            closeAll = yes;
                        }

                        ifMemEqual(buffer + 10, "system", 6) {
                            RPrintf(">> Execute %s", buffer + 17);
                            system(buffer + 17);
                        }

                    } else {
                        RPrintf("[E] Bad user key on %s:%u\n", address, port);
                    }
                }
                connectionState = networkConnectionClosedConst;
            } else if (connectionState == networkOperationErrorConst) {
                RError2("[E] Receive on configurator connection, from %s:%u", current, address, port);
            }
        }

        deleter(current, RSocket);
    }

    deleter(configurator, RSocket);

    deallocator(delegate);
    p(RTCPHandler)(server);
    $(server, m(terminate, RTCPHandler)));
    deleter(server,        RTCPHandler);

    exit:
    endSockets();
    endRay();
}
```

