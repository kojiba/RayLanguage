cmake_minimum_required(VERSION 2.8.4)
project(RayLanguage)

# Configure project
set(EXECUTABLE_OUTPUT_PATH ${RayLanguage_SOURCE_DIR}/bin)


# sub-sources flags
set(RVM            NO)
set(RayInterpreter NO)
set(DebugFlag      NO) # NO - for build library
set(copyResources  NO)
set(RayPreloadFlag NO) # lib must be compiled with RAY_LIBRARY_PRELOAD_ALLOCATOR in RayConfig


if(NOT ${DebugFlag})
    set(BUILD_SHARED_LIBS      ON)
    # Display Build path
    set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
    set(LIBRARY_NAME    Ray)
endif()

set(EXECUTABLE_NAME RayLanguage)

include_directories(Classes)

set(HEADER_FILES # Worker headers
                 Classes/RayFoundation/RayConfig.h
                 Classes/RayFoundation/RayBase.h
                 Classes/RayFoundation/RSystem.h
                 Classes/RayFoundation/RayCheckFoundation.h
                 Classes/RayFoundation/RColors.h
                 Classes/RayFoundation/RSyntax.h
                 Classes/RayFoundation/RBasics/RBasics.h
                 Classes/RayFoundation/RErrors.h

                 # Memory operations
                 Classes/RayFoundation/RMemoryOperations/RByteOperations.h
                 Classes/RayFoundation/RMemoryOperations/RData.h
                 Classes/RayFoundation/RMemoryOperations/RSandBox.h
                 Classes/RayFoundation/RMemoryOperations/RAutoPool.h

                 # Containers
                 Classes/RayFoundation/RContainers/RArray.h
                 Classes/RayFoundation/RContainers/RArray_Blocks.h
                 Classes/RayFoundation/RContainers/RArray_Parallel.h
                 Classes/RayFoundation/RContainers/RList.h
                 Classes/RayFoundation/RContainers/RDictionary.h
                 Classes/RayFoundation/RContainers/RBuffer.h

                 # Strings
                 Classes/RayFoundation/RString/RString.h
                 Classes/RayFoundation/RString/RString_Char.h
                 Classes/RayFoundation/RString/RString_File.h
                 Classes/RayFoundation/RString/RString_Numbers.h
                 Classes/RayFoundation/RString/RString_UTF8.h
                 Classes/RayFoundation/RString/RString_Consts.h


                 # Encoding
                 Classes/RayFoundation/REncoding/RBase64.h
                 Classes/RayFoundation/REncoding/purge.h
                 Classes/RayFoundation/REncoding/evasion.h
                 Classes/RayFoundation/REncoding/PurgeEvasionUtils.h
                 Classes/RayFoundation/REncoding/PurgeEvasionUtilsRay.h

                 # Others
                 Classes/RayFoundation/RClassTable/RClassTable.h
                 Classes/RayFoundation/RClassTable/RClassNamePair.h

                 Classes/RayFoundation/RThread/RThread.h
                 Classes/RayFoundation/RThread/RThreadPool.h

                 # Networking
                 Classes/RayFoundation/RNetwork/RSocket.h
                 Classes/RayFoundation/RNetwork/RTCPHandler.h

                 # Core header
                 Classes/RayFoundation/RayFoundation.h

                 # Utils
                 Classes/RayFoundation/Utils/Utils.h
                 Classes/RayFoundation/Utils/PurgeEvasionConnection.h
                 Classes/RayFoundation/Utils/PurgeEvasionTCPHandler.h
                 Classes/RayFoundation/Utils/PurgeEvasionParallel.h)

set(SOURCE_FILES Classes/RayFoundation/RayBase.c
                 Classes/RayFoundation/RBasics/RBasics.c

                 # Containers
                 Classes/RayFoundation/RContainers/RArray.c
                 Classes/RayFoundation/RContainers/RArray_Blocks.c
                 Classes/RayFoundation/RContainers/RArray_Parallel.c
                 Classes/RayFoundation/RContainers/RList.c
                 Classes/RayFoundation/RContainers/RDictionary.c
                 Classes/RayFoundation/RContainers/RBuffer.c

                 # Memory operations
                 Classes/RayFoundation/RMemoryOperations/RByteOperations.c
                 Classes/RayFoundation/RMemoryOperations/RData.c
                 Classes/RayFoundation/RMemoryOperations/RSandBox.c
                 Classes/RayFoundation/RMemoryOperations/RAutoPool.c

                 # Strings
                 Classes/RayFoundation/RString/RString.c
                 Classes/RayFoundation/RString/RString_Char.c
                 Classes/RayFoundation/RString/RString_File.c
                 Classes/RayFoundation/RString/RString_Numbers.c
                 Classes/RayFoundation/RString/RString_UTF8.c
                 Classes/RayFoundation/RString/RString_Consts.c

                 # Encoding
                 Classes/RayFoundation/REncoding/RBase64.c
                 Classes/RayFoundation/REncoding/purge.c
                 Classes/RayFoundation/REncoding/evasion.c
                 Classes/RayFoundation/REncoding/PurgeEvasionUtils.c
                 Classes/RayFoundation/REncoding/PurgeEvasionUtilsRay.c

                 # Others
                 Classes/RayFoundation/RClassTable/RClassTable.c
                 Classes/RayFoundation/RClassTable/RClassNamePair.c

                 Classes/RayFoundation/RThread/RThread.c
                 Classes/RayFoundation/RThread/RThreadPool.c

                 # Networking
                 Classes/RayFoundation/RNetwork/RSocket.c
                 Classes/RayFoundation/RNetwork/RTCPHandler.c

                 # Utils
                 Classes/RayFoundation/Utils/Utils.c
                 Classes/RayFoundation/Utils/PurgeEvasionConnection.c
                 Classes/RayFoundation/Utils/PurgeEvasionTCPHandler.c
                 Classes/RayFoundation/Utils/PurgeEvasionParallel.c)

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

add_executable(${EXECUTABLE_NAME} Classes/main.c Classes/Tests.h
                                                 Classes/Tests.c

                                                 Classes/RayChat.h
                                                 Classes/RayChat.c

                                                 ${RVM_HEADERS}
                                                 ${RVM_SOURCES}
                                                 ${RINTERPRETER_HEADERS}
                                                 ${RINTERPRETER_SOURCES}
                                                 ${DEBUG_SOURCES}
                                                 ${DEBUG_HEADERS})

                                                 target_link_libraries(
                                                         ${EXECUTABLE_NAME}
                                                     )

if(copyResources)
    file(COPY Resources     DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif()

if(NOT ${DebugFlag})
#    file(COPY ${RayLanguage_SOURCE_DIR}/Classes/RayFoundation DESTINATION ${EXECUTABLE_OUTPUT_PATH})

    if(${RayPreloadFlag})
        SET( ENV{LD_PRELOAD}   ${LIBRARY_OUTPUT_PATH}/${LIBRARY_NAME})
    endif()

    target_link_libraries(
        ${EXECUTABLE_NAME}
        ${LIBRARY_NAME}
    )

    if(WIN32)
        target_link_libraries(${LIBRARY_NAME} wsock32 ws2_32)
    else()
        target_link_libraries(${EXECUTABLE_NAME} curses)
        target_link_libraries(${LIBRARY_NAME} pthread)
    endif()

    set_target_properties(${EXECUTABLE_NAME} PROPERTIES PREFIX "")

    install( TARGETS ${EXECUTABLE_NAME}
             RUNTIME DESTINATION bin COMPONENT libraries
             LIBRARY DESTINATION lib COMPONENT libraries
             ARCHIVE DESTINATION lib/static COMPONENT libraries)
else()
    if(WIN32)
        set(ADDITIONAL_OS_LIBRARIES wsock32 ws2_32)

    else()
        set(ADDITIONAL_OS_LIBRARIES pthread)

    endif()
    target_link_libraries(${LIBRARY_NAME} ${ADDITIONAL_OS_LIBRARIES})
endif()



