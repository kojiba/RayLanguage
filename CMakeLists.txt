cmake_minimum_required(VERSION 2.8.4)
project(RayLanguage)

set(EXECUTABLE_OUTPUT_PATH ${RayLanguage_SOURCE_DIR}/bin)
file(COPY Resources DESTINATION ${EXECUTABLE_OUTPUT_PATH})

# RVM -----------------------------------------------------------------------
set(RVM_HEADERS
               Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.h
               Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h
               Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.h)

set(RVM_SOURCES
               Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.c
               Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.c
               Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.c)
# RVM -----------------------------------------------------------------------

# RInterpreter --------------------------------------------------------------
set(RINTERPRETER_HEADERS # Interpreter

                        Classes/RInterpreter/RIProperties/RIProperties.h
                        Classes/RInterpreter/RIMethods/RIMethods.h
                        Classes/RInterpreter/RIClass/RIClass.h

                        Classes/RInterpreter/RInterpreter.h)


set(RINTERPRETER_SOURCES # Interpreter
                         Classes/RInterpreter/RIProperties/RIProperties.c
                         Classes/RInterpreter/RIMethods/RIMethods.c
                         Classes/RInterpreter/RIClass/RIClass.c

                         Classes/RInterpreter/RInterpreter.c)

# RInterpreter --------------------------------------------------------------

set(HEADER_FILES # Worker headers
                 Classes/RayFoundation/RayBase.h
                 Classes/RayFoundation/RSystem.h
                 Classes/RayFoundation/RayCheckFoundation.h
                 Classes/RayFoundation/RColors.h
                 Classes/RayFoundation/RSyntax.h
                 Classes/RayFoundation/RBasics/RBasics.h

                 # Containers
                 Classes/RayFoundation/RContainers/RArray.h
                 Classes/RayFoundation/RContainers/RDictionary.h
                 Classes/RayFoundation/RContainers/RStringDictionary.h
                 Classes/RayFoundation/RContainers/RBuffer.h

                 # Strings
                 Classes/RayFoundation/RCString/RCString.h
                 Classes/RayFoundation/RCString/RCString_Numbers.h

                 # Memory operations
                 Classes/RayFoundation/RMemoryOperations/RByteOperations.h
                 Classes/RayFoundation/RMemoryOperations/RSandBox.h
                 Classes/RayFoundation/RMemoryOperations/RAutoPool.h

                 # Encoding
                 Classes/RayFoundation/REncoding/RBase64.h

                 # Others
                 Classes/RayFoundation/RFloatingEnum/RFloatingEnum.h
                 Classes/RayFoundation/RClassTable/RClassTable.h
                 Classes/RayFoundation/RClassTable/RClassNamePair.h

                 # Core header
                 Classes/RayFoundation/RayFoundation.h

                 # Tests
                 Classes/Tests.h)

set(SOURCE_FILES Classes/RayFoundation/RBasics/RBasics.c

                 # Containers
                 Classes/RayFoundation/RContainers/RArray.c
                 Classes/RayFoundation/RContainers/RDictionary.c
                 Classes/RayFoundation/RContainers/RStringDictionary.c
                 Classes/RayFoundation/RContainers/RBuffer.c

                 # Strings
                 Classes/RayFoundation/RCString/RCString.c
                 Classes/RayFoundation/RCString/RCString_Numbers.c

                 # Memory operations
                 Classes/RayFoundation/RMemoryOperations/RByteOperations.c
                 Classes/RayFoundation/RMemoryOperations/RSandBox.c
                 Classes/RayFoundation/RMemoryOperations/RAutoPool.c

                 # Encoding
                 Classes/RayFoundation/REncoding/RBase64.c

                 # Others
                 Classes/RayFoundation/RClassTable/RClassTable.c
                 Classes/RayFoundation/RClassTable/RClassNamePair.c
                 Classes/RayFoundation/RFloatingEnum/RFloatingEnum.c

                 # Tests
                 Classes/Tests.c

                 Classes/main.c)

add_executable(RayLanguage ${HEADER_FILES} ${RINTERPRETER_HEADERS} ${RVM_HEADERS} ${SOURCE_FILES} ${RINTERPRETER_SOURCES} ${RVM_SOURCES})

#add_executable(RayInterpreter ${RINTERPRETER_HEADERS} ${RINTERPRETER_SOURCES})

#add_executable(RVM ${RVM_HEADERS} ${RVM_SOURCES})



