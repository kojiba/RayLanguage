/**
 * RayFoundation.h
 * A ray of light in the realm of darkness.
 * Some additions to C.
 * If You don't like it, You can preprocess file, to get pure-C code.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 */

#ifndef __RAY_FOUNDATION__
#define __RAY_FOUNDATION__

#define RAY_SHORT_DEBUG 451

#include <time.h>
#include "RayBase.h"
#include "RayCheckFoundation.h"

// working defines
#define _TOSTRING(x)                                      #x
#define toString(x)                                       _TOSTRING(x)        // gets c-string from define or some code

#define _splitUp(one, two)                                one##two
#define splitUp(one, two)                                 _splitUp(one, two)  // splits up two words: splitUp(Hello,World) -> HelloWorld

#define initRClock()                                      clock_t start = clock();\
                                                          int msec = 0;
#define tickRClock()                                      start = clock() - start; \
                                                          msec = start * 1000 / CLOCKS_PER_SEC; \
                                                          RPrintf("Time taken %d seconds %d milliseconds\n", msec / 1000, msec % 1000);

// typedefs
typedef void* pointer;
typedef uint8_t byte;

// declarations
#define class(className)                                  typedef struct className { \
                                                          uint64_t classId;
#define protocol(protocolName)                            typedef struct protocolaName {

#define discipleOf(className)                             className *splitUp(master, splitUp(className, Object));
#define endOf(className)                                  } className;

#define allocator(className)                              RAlloc(sizeof(className))
#define deallocator(object)                               RFree(object);
#define master(object, masterClassName)                   object->splitUp(master,splitUp(masterClassName, Object))

#define method(returnValue, methodName, className)        eturnValue splitUp(methodName, className)(className *object
#define virtualMethod(returnValue, methodName, className) returnValue (*splitUp(splitUp(virtualMethod,methodName),splitUp(Of,className)))(struct className *object

#define constructor(className)                            className* splitUp(constructorOf,className) (className *object
#define destructor(className)                             void splitUp(destructorOf,className) (className *object)
#define printer(className)                                void splitUp(printerOf,className) (className *object)
#define singleton(className)                              className* splitUp(singletonOf,className)(void)
#define staticMethod(returnValue, methodName, className)  returnValue splitUp(splitUp(staticMethod, methodName), splitUp(Of, className))(className *deprecatedObject

// calls
#define c(className)                                      splitUp(constructorOf, className)                                  // constructor function name
#define d(className)                                      splitUp(destructorOf, className)                                   // destructor function name
#define p(className)                                      splitUp(printerOf, className)                                      // printer function name
#define m(methodName, className)                          splitUp(methodName,className)                                      // some method function name
#define sm(methodName, className)                         splitUp(splitUp(staticMethod,methodName),splitUp(Of,className))    // static method function name
#define vm(methodName, className)                         object->splitUp(splitUp(virtualMethod,methodName),splitUp(Of,className))
#define singletonCall(className)                          splitUp(singletonOf,className)()                                   // singleton call

#define $(object, methodName)                             methodName(object

// cases/if/for ...
#define forAll(iterator, count)                           for(iterator = 0; iterator < count; ++iterator)
#define fromStartForAll(iterator, start, count)           for(iterator = start; iterator < start + count; ++iterator)

// naming rules
#define members
#define methods

#endif /*__RAY_FOUNDATION__*/