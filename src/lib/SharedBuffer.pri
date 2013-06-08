QMAKE_CXXFLAGS += -std=c++11

LIBS += \
    -llog4cxx \
    -lboost_timer \
    -lboost_thread

HEADERS += \
    SharedBufferGlobal.h \
    SharedBufferWriter.h \
    SignalPack.h \
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
    domain/Buffer.h

SOURCES += \
    SharedBufferWriter.cpp \
    BufferWriter.cpp \
    LowLevelBufferHandler.cpp \
    SharedBufferReader.cpp \
    SharedBufferServer.cpp \
    AbstractSharedBufferHandler.cpp \
    QtBasedSharedMemory.cpp \
    BeanFactory.cpp
