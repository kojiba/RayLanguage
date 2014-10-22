cmake_minimum_required(VERSION 2.8.4)
project(RayLanguage)

set(EXECUTABLE_OUTPUT_PATH ${RayLanguage_SOURCE_DIR}/bin)

set(HEADER_FILES Classes/RayFoundation/RayBase.h
                 Classes/RayFoundation/RayCheckFoundation.h
                 Classes/RayFoundation/RSyntax.h
                 Classes/RayFoundation/RBasics/RBasics.h
                 Classes/RayFoundation/RArray/RArray.h
                 Classes/RayFoundation/RClassTable/RClassTable.h
                 Classes/RayFoundation/RClassTable/RClassNamePair/RClassNamePair.h
                 Classes/RayFoundation/RCString/RCString.h
                 Classes/RayFoundation/RDictionary/RDictionary.h
                 Classes/RayFoundation/RStringDictionary/RStringDictionary.h
                 Classes/RayFoundation/RByteOperations/RByteOperations.h
                 Classes/RayFoundation/RFloatingEnum/RFloatingEnum.h
                 Classes/RayFoundation/RayFoundation.h

                 # RVM
                 Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.h
                 Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h
                 Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.h)

set(SOURCE_FILES Classes/RayFoundation/RBasics/RBasics.c
                 Classes/RayFoundation/RArray/RArray.c
                 Classes/RayFoundation/RClassTable/RClassTable.c
                 Classes/RayFoundation/RClassTable/RClassNamePair/RClassNamePair.c
                 Classes/RayFoundation/RCString/RCString.c
                 Classes/RayFoundation/RDictionary/RDictionary.c
                 Classes/RayFoundation/RStringDictionary/RStringDictionary.c
                 Classes/RayFoundation/RByteOperations/RByteOperations.c
                 Classes/RayFoundation/RFloatingEnum/RFloatingEnum.c

                 # RVM
                 Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.c
                 Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.c
                 Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.c

                 Classes/main.c)

add_executable(RayLanguage ${HEADER_FILES} ${SOURCE_FILES})


