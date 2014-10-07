/**
 * RayFoundation.h
 * A ray of light in the realm of darkness.
 * Some additions to C.
 * If You don't like it, You can preprocess file, to get pure-C code.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 */

#ifndef __RAY_FOUNDATION__
#define __RAY_FOUNDATION__

#define RAY_SHORT_DEBUG 451

#include "RayBase.h"
#include "RayCheckFoundation.h"

// working defines
#define _TOSTRING(x)                                      #x
#define toString(x)                                       _TOSTRING(x)        // gets c-string from define or some code

#define _concatenate(one, two)                            one##two
#define concatenate(one, two)                             _concatenate(one, two)  // concatenate two words: concatenate(Hello, World) -> HelloWorld

#define initRClock()                                      clock_t start = clock(); \
                                                          clock_t stop = 0; \
                                                          clock_t diff = 0;

#define tickRClock()                                      stop = clock(); \
                                                          diff = stop - start; \
                                                          start = clock(); \
                                                          RPrintf("Time taken %d seconds %d milliseconds\n", diff / 1000 / CLOCKS_PER_SEC, (diff / CLOCKS_PER_SEC) % 1000 );

// typedefs
typedef void* pointer;
typedef uint8_t byte;

// declarations
#define class(className)                                  typedef struct className { \
                                                          uint64_t classId;
#define protocol(protocolName)                            typedef struct protocolaName {

#define discipleOf(className)                             className *concatenate(master, concatenate(className, Object));
#define endOf(className)                                  } className;

#define allocator(className)                              RAlloc(sizeof(className))
#define deallocator(object)                               RFree(object);

#define method(returnValue, methodName, className)        returnValue concatenate(methodName, className)(className *object
#define virtualMethod(returnValue, methodName, className) returnValue (*concatenate(concatenate(virtualMethod,methodName),concatenate(Of,className)))(struct className *object

#define constructor(className)                            className* concatenate(constructorOf,className) (className *object
#define destructor(className)                             void concatenate(destructorOf,className) (className *object)
#define printer(className)                                void concatenate(printerOf,className) (className *object)
#define singleton(className)                              className* concatenate(singletonOf,className)(void)
#define staticMethod(returnValue, methodName, className)  returnValue concatenate(concatenate(staticMethod, methodName), concatenate(Of, className))(className *deprecatedObject

// calls
#define c(className)                                      concatenate(constructorOf, className)                                  // constructor function name
#define d(className)                                      concatenate(destructorOf, className)                                   // destructor function name
#define p(className)                                      concatenate(printerOf, className)                                      // printer function name
#define m(methodName, className)                          concatenate(methodName,className)                                      // some method function name
#define sm(methodName, className)                         concatenate(concatenate(staticMethod,methodName),concatenate(Of,className))    // static method function name
#define vm(methodName, className)                         object->concatenate(concatenate(virtualMethod,methodName),concatenate(Of,className))
#define master(object, masterClassName)                   object->concatenate(master,concatenate(masterClassName, Object))           // call to masterClassObject
#define singletonCall(className)                          concatenate(singletonOf,className)()                                   // singleton call

#define $(object, methodName)                             methodName(object

// cases/if/for ...
#define forAll(iterator, count)                           for(iterator = 0; iterator < count; ++iterator)
#define fromStartForAll(iterator, start, count)           for(iterator = start; iterator < start + count; ++iterator)

// naming rules
#define members
#define methods

#endif /*__RAY_FOUNDATION__*/