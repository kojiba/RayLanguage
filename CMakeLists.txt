cmake_minimum_required(VERSION 2.8.4)
project(RayLanguage)

set(SOURCE_FILES Classes/main.c
                 Classes/RayFoundation/RayCheckFoundation.h
                 Classes/RayFoundation/RayFoundation.h

                 Classes/RayFoundation/RDynamicArray/RDynamicArray.h
                 Classes/RayFoundation/RDynamicArray/RDynamicArray.c

                 Classes/RayFoundation/RClassTable/RClassTable.c
                 Classes/RayFoundation/RClassTable/RClassTable.h

                 Classes/RayFoundation/RClassTable/RClassNamePair/RClassNamePair.c
                 Classes/RayFoundation/RClassTable/RClassNamePair/RClassNamePair.h

                 Classes/RayFoundation/RCString/RCString.c
                 Classes/RayFoundation/RCString/RCString.h

                 Classes/RayFoundation/Function/Function.c
                 Classes/RayFoundation/Function/Function.h)

add_executable(RayLanguage ${SOURCE_FILES})