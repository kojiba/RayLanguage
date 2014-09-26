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

                 Classes/RayFoundation/Function/Function.h
                 Classes/RayFoundation/Function/Function.c

                 Classes/RayFoundation/RArray/RCompareDelegate/RCompareDelegate.h
                 Classes/RayFoundation/RArray/RCompareDelegate/RCompareDelegate.c

                 Classes/RayFoundation/RDictionary/RDictionary.h
                 Classes/RayFoundation/RDictionary/RDictionary.c)

add_executable(RayLanguage ${SOURCE_FILES})