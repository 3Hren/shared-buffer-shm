cmake_minimum_required(VERSION 2.8)
SET(PROJECT sharbuf-server)

SET(UTILS_PATH ../../utils)
SET(HEADERS
    SharedBufferOptionParser.h
    ${UTILS_PATH}/OptionPrinter.h
    ${UTILS_PATH}/CustomOptionDescription.h
)

SET(MOC)

SET(SOURCES
    main.cpp
    SharedBufferOptionParser.cpp
    ${UTILS_PATH}/OptionPrinter.cpp
    ${UTILS_PATH}/CustomOptionDescription.cpp
)

SET(BOOST_COMPONENTS
    system
    filesystem
    regex
    program_options
)

SET(Boost_USE_MULTITHREADED ON)

add_definitions(-std=c++11)

PROJECT(${PROJECT})
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
qt4_wrap_cpp(MOC_SOURCES ${MOC})

find_package(Threads)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(${PROJECT_BINARY_DIR} ../../lib ../../utils)
add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES})
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT} ${QT_LIBRARIES} ${Boost_LIBRARIES} log4cxx sharbuf)
SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES ENABLE_EXPORTS true)

INSTALL(
    TARGETS
        sharbuf-server
    RUNTIME DESTINATION bin COMPONENT runtime
)

INSTALL(
    FILES
        ../../../other/log.properties
    DESTINATION share/sharbuf
    COMPONENT runtime
)
