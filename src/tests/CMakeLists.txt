cmake_minimum_required(VERSION 2.8)
SET(PROJECT sharbuf-tests)

SET(HEADERS
    Global.h
    Mocks.h
)

SET(SOURCES
    _Main.cpp
    test_BufferHandler.cpp
    test_SharedBufferHandler.cpp
)

SET(BOOST_COMPONENTS
)

SET(GOOGLE_LIBRARIES
    gtest
    gmock
)

SET(Boost_USE_MULTITHREADED ON)

add_definitions(-std=c++11)

PROJECT(${PROJECT})
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

find_package(Threads)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
include_directories(../lib)
add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT} ${QT_LIBRARIES} ${Boost_LIBRARIES} ${GOOGLE_LIBRARIES} sharbuf)
SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES ENABLE_EXPORTS true)
