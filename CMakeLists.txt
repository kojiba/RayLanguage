cmake_minimum_required(VERSION 2.8.4)
project(RayLanguage)

# Configure project
set(EXECUTABLE_OUTPUT_PATH ${RayLanguage_SOURCE_DIR}/bin)

# sub-sources flags
set(RVM            NO)
set(RayInterpreter NO)
set(DebugFlag      NO) # NO - for build library


if(NOT ${DebugFlag})
    set(BUILD_SHARED_LIBS      ON)
    # Display Build path
    set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
    set(LIBRARY_NAME    Ray)
endif()

set(EXECUTABLE_NAME RayLanguage)

include_directories(Classes/RayFoundation)
include_directories(Classes/RayFoundation/RBasics)
include_directories(Classes/RayFoundation/RClassTable)
include_directories(Classes/RayFoundation/RContainers)
include_directories(Classes/RayFoundation/RCString)
include_directories(Classes/RayFoundation/RCustomIO)
include_directories(Classes/RayFoundation/REncoding)
include_directories(Classes/RayFoundation/RFloatingEnum)
include_directories(Classes/RayFoundation/RMemoryOperations)
include_directories(Classes/RayFoundation/RThread)
include_directories(Classes/RayFoundation/RNetwork)

set(HEADER_FILES # Worker headers
                 Classes/RayFoundation/RayConfig.h
                 Classes/RayFoundation/RayBase.h
                 Classes/RayFoundation/RSystem.h
                 Classes/RayFoundation/RayCheckFoundation.h
                 Classes/RayFoundation/RColors.h
                 Classes/RayFoundation/RSyntax.h
                 Classes/RayFoundation/RBasics/RBasics.h

                 # Containers
                 Classes/RayFoundation/RContainers/RArray.h
                 Classes/RayFoundation/RContainers/RArray_Parallel.h
                 Classes/RayFoundation/RContainers/Rlist.h
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

                 Classes/RayFoundation/RThread/RThread.h
                 Classes/RayFoundation/RThread/RThreadPool.h

                 # Networking
                 Classes/RayFoundation/RNetwork/RSocket.h
                 Classes/RayFoundation/RNetwork/RReceiver.h
                 Classes/RayFoundation/RNetwork/RSender.h

                 # Core header
                 Classes/RayFoundation/RayFoundation.h

                 # Tests
                 Classes/Tests.h)

set(SOURCE_FILES Classes/RayFoundation/RayBase.c
                 Classes/RayFoundation/RBasics/RBasics.c

                 # Containers
                 Classes/RayFoundation/RContainers/RArray.c
                 Classes/RayFoundation/RContainers/RArray_Parallel.c
                 Classes/RayFoundation/RContainers/RList.c
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

                 Classes/RayFoundation/RThread/RThread.c
                 Classes/RayFoundation/RThread/RThreadPool.c

                 # Networking
                 Classes/RayFoundation/RNetwork/RReceiver.c
                 Classes/RayFoundation/RNetwork/RSender.c

                 # Tests
                 Classes/Tests.c)

if(NOT ${DebugFlag})
    add_library(
        ${LIBRARY_NAME}
        SHARED
        ${HEADER_FILES}
        ${SOURCE_FILES}
    )
else()
    set(DEBUG_SOURCES ${SOURCE_FILES})
    set(DEBUG_HEADERS ${HEADER_FILES})
endif()

if(RVM) # RVM -----------------------------------------------------------------------
    set(RVM_HEADERS
                   Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.h
                   Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.h
                   Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.h)

    set(RVM_SOURCES
                   Classes/RVirtualMachine/RVirtualFunction/RVirtualFunction.c
                   Classes/RVirtualMachine/RVirtualCompiler/RVirtualCompiler.c
                   Classes/RVirtualMachine/RVirtualMachine/RVirtualMachine.c)

    add_executable(RVM ${RVM_HEADERS} ${RVM_SOURCES})

    target_link_libraries(
        RVM
        ${LIBRARY_NAME}
    )
else()
    set(RVM_HEADERS)
    set(RVM_SOURCES)
endif() # RVM -----------------------------------------------------------------------

if(RayInterpreter) # RInterpreter ---------------------------------------------------
    set(RINTERPRETER_HEADERS # Interpreter

                            Classes/RInterpreter/RInterpereterConsts.h
                            Classes/RInterpreter/RIProperties/RIProperties.h
                            Classes/RInterpreter/RIMethods/RIMethods.h
                            Classes/RInterpreter/RIClass/RIClass.h

                            Classes/RInterpreter/RInterpreter.h)


    set(RINTERPRETER_SOURCES # Interpreter
                             Classes/RInterpreter/RIProperties/RIProperties.c
                             Classes/RInterpreter/RIMethods/RIMethods.c
                             Classes/RInterpreter/RIClass/RIClass.c

                             Classes/RInterpreter/RInterpreter.c)

    add_executable(RayInterpreter ${RINTERPRETER_HEADERS} ${RINTERPRETER_SOURCES})
    target_link_libraries(
        RayInterpreter
        ${LIBRARY_NAME}
    )
else()
    set(RINTERPRETER_HEADERS)
    set(RINTERPRETER_SOURCES)
endif() # RInterpreter ---------------------------------------------------

add_executable(${EXECUTABLE_NAME} Classes/main.c ${RVM_HEADERS}
                                                 ${RVM_SOURCES}
                                                 ${RINTERPRETER_HEADERS}
                                                 ${RINTERPRETER_SOURCES}
                                                 ${DEBUG_SOURCES}
                                                 ${DEBUG_HEADERS}
)

set(copyResources NO)

if(copyResources)
    file(COPY Resources DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif()

if(NOT ${DebugFlag})
    file(COPY ${HEADER_FILES} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/RayFoundation)

    target_link_libraries(
        ${EXECUTABLE_NAME}
        ${LIBRARY_NAME}
    )

    if(WIN32)
      target_link_libraries(${LIBRARY_NAME} wsock32 ws2_32)
    endif()

    set_target_properties(${EXECUTABLE_NAME} PROPERTIES PREFIX "")
    install( TARGETS ${EXECUTABLE_NAME}
             RUNTIME DESTINATION bin COMPONENT libraries
             LIBRARY DESTINATION lib COMPONENT libraries
             ARCHIVE DESTINATION lib/static COMPONENT libraries)
else()
    if(WIN32)
      target_link_libraries(${EXECUTABLE_NAME} wsock32 ws2_32)
    endif()
endif()



