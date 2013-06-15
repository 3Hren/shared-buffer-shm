cmake_minimum_required(VERSION 2.8)
SET(PROJECT sharbuf)

SET(HEADERS
    ThreadSafeQueue.h
    AbstractSharedBufferHandler.h
    BeanFactory.h
    BufferWriter.h
    LowLevelBufferHandler.h
    QtBasedSharedMemory.h
    SharedBufferException.h
    SharedBufferGlobal.h
    SharedBufferReader.h
    SharedBufferServer.h
    SharedBufferWriter.h
    SharedMemory.h
    ThreadSafeQueue.h
    domain/Buffer.h
    domain/Internal.h
    domain/MetaData.h
    domain/SignalPack.h
)

SET(MOC
    SharedBufferServer.h
)

SET(SOURCES
    AbstractSharedBufferHandler.cpp
    BeanFactory.cpp
    BufferWriter.cpp
    LowLevelBufferHandler.cpp
    QtBasedSharedMemory.cpp
    SharedBufferReader.cpp
    SharedBufferServer.cpp
    SharedBufferWriter.cpp
)

SET(BOOST_COMPONENTS
    thread
    system
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
include_directories(. ${PROJECT_BINARY_DIR})
add_library(${PROJECT} SHARED ${HEADERS} ${SOURCES} ${MOC_SOURCES})
target_link_libraries(${PROJECT} ${CMAKE_THREAD_LIBS_INIT} ${QT_LIBRARIES} ${Boost_LIBRARIES} log4cxx)
SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES ENABLE_EXPORTS true)
