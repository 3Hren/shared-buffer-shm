QMAKE_CXXFLAGS += -std=c++11

LIBS += \
    -llog4cxx \
    -lboost_timer \
    -lboost_thread

HEADERS += \
    SharedBufferGlobal.h \
    _SharedBufferStorageClient.h \
    _Pusher.h \
    _Dumper.h \
    SharedBufferWriter.h \
    SignalPack.h \    
    SharedBufferException.h \
    BufferWriter.h \
    LowLevelBufferHandler.h \
    SharedBufferReader.h \
    SharedBufferServer.h \
    AbstractSharedBufferHandler.h \
    SharedMemory.h \
    QtBasedSharedMemory.h

SOURCES += \
    _SharedBufferStorageClient.cpp \
    _Pusher.cpp \
    _Dumper.cpp \
    SharedBufferWriter.cpp \
    BufferWriter.cpp \
    LowLevelBufferHandler.cpp \
    SharedBufferReader.cpp \
    SharedBufferServer.cpp \
    AbstractSharedBufferHandler.cpp \
    QtBasedSharedMemory.cpp
