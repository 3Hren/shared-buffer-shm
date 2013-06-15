ROOT_PATH = ../..
TEMPLATE = lib
TARGET = pysharbuf
DESTDIR = $${ROOT_PATH}/lib

VERSION = 0.2.3
LIB_PATH = $${ROOT_PATH}/src/lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

QMAKE_CXXFLAGS += -std=c++11

LIBS += \
    -llog4cxx \
    -lboost_timer \
    -lboost_thread \
    -lboost_python-py27

INCLUDEPATH += \
    $${LIB_PATH} \
    /usr/include/python2.7/

HEADERS += \
    $${LIB_PATH}/SharedBufferGlobal.h \
    $${LIB_PATH}/SharedBufferException.h \
    $${LIB_PATH}/LowLevelBufferHandler.h \
    $${LIB_PATH}/SharedBufferReader.h \
    $${LIB_PATH}/AbstractSharedBufferHandler.h \
    $${LIB_PATH}/SharedMemory.h \
    $${LIB_PATH}/QtBasedSharedMemory.h \
    $${LIB_PATH}/domain/Buffer.h \
    $${LIB_PATH}/domain/MetaData.h

SOURCES += \
    $${LIB_PATH}/LowLevelBufferHandler.cpp \
    $${LIB_PATH}/SharedBufferReader.cpp \
    $${LIB_PATH}/AbstractSharedBufferHandler.cpp \
    $${LIB_PATH}/QtBasedSharedMemory.cpp \
    Wrap.cpp
