QMAKE_CXXFLAGS += -std=c++11

QT -= gui

LIBS += \
    -llog4cxx \
    -lboost_timer \
    -lboost_thread

HEADERS += \
    SharedBufferGlobal.h \
    SharedBufferWriter.h \    
    SharedBufferException.h \
    BufferWriter.h \
    LowLevelBufferHandler.h \
    SharedBufferReader.h \
    SharedBufferServer.h \
    AbstractSharedBufferHandler.h \
    SharedMemory.h \
    QtBasedSharedMemory.h \
    BeanFactory.h \
    ThreadSafeQueue.h \
    domain/SignalPack.h \
    domain/Buffer.h \
    domain/MetaData.h \
    domain/Internal.h

SOURCES += \
    SharedBufferWriter.cpp \
    BufferWriter.cpp \
    LowLevelBufferHandler.cpp \
    SharedBufferReader.cpp \
    SharedBufferServer.cpp \
    AbstractSharedBufferHandler.cpp \
    QtBasedSharedMemory.cpp \
    BeanFactory.cpp
