QMAKE_CXXFLAGS += -std=c++11

LIBS += \
    -llog4cxx \
    -lboost_timer \
    -lboost_thread

HEADERS += \
    LowLevelBufferManager.h \
    SharedBufferGlobal.h \
    SharedBufferStorageServer.h \
    _Pusher.h \
    _Dumper.h \
    SharedBufferStorageClient.h \
    BufferManager.h \
    SharedBufferWriter.h \
    SharedBufferNotAttachedException.h \
    ../../lib/SignalPack.h

SOURCES += \
    SharedBufferStorageServer.cpp \
    _Pusher.cpp \
    _Dumper.cpp \
    SharedBufferStorageClient.cpp \
    LowLevelBufferManager.cpp \
    BufferManager.cpp \
    SharedBufferWriter.cpp
