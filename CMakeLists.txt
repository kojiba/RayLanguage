cmake_minimum_required(VERSION 2.8.4)
project(RayLanguage)

set(SOURCE_FILES Classes/main.c
                 Classes/RayFoundation/RayBase.h
                 Classes/RayFoundation/RayCheckFoundation.h
                 Classes/RayFoundation/RayFoundation.h

                 Classes/RayFoundation/RArray/RArray.h
                 Classes/RayFoundation/RArray/RArray.c

                 Classes/RayFoundation/RClassTable/RClassTable.h
                 Classes/RayFoundation/RClassTable/RClassTable.c

                 Classes/RayFoundation/RClassTable/RClassNamePair/RClassNamePair.h
                 Classes/RayFoundation/RClassTable/RClassNamePair/RClassNamePair.c

                 Classes/RayFoundation/RCString/RCString.h
                 Classes/RayFoundation/RCString/RCString.c
                 Classes/RayFoundation/RCString/Encoding/Base64.h
                 Classes/RayFoundation/RCString/Encoding/Base64.c

                 Classes/RayFoundation/RArray/RCompareDelegate/RCompareDelegate.h
                 Classes/RayFoundation/RArray/RCompareDelegate/RCompareDelegate.c

                 Classes/RayFoundation/RDictionary/RDictionary.h
                 Classes/RayFoundation/RDictionary/RDictionary.c

                 Classes/RayFoundation/RStringDictionary/RStringDictionary.h
                 Classes/RayFoundation/RStringDictionary/RStringDictionary.c

                 Classes/RayFoundation/RByteOperations/RByteOperations.h
                 Classes/RayFoundation/RByteOperations/RByteOperations.c

                 # RVM
                 Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.h
                 Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.c

                 Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h
                 Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.c

                 Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.h
                 Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.c)


#file(COPY Resources DESTINATION ${CMAKE_BINARY_DIR})
add_executable(RayLanguage ${SOURCE_FILES})
