QMAKE_CXXFLAGS += -std=c++11

LIBS += \
    -llog4cxx \
    -lboost_timer

HEADERS += \
    BufferManager.h \
    SharedBufferGlobal.h \
    SharedBufferStorageServer.h \
    Pusher.h \
    Dumper.h \
    SharedBufferStorageClient.h

SOURCES += \
    BufferManager.cpp \
    SharedBufferStorageServer.cpp \
    Pusher.cpp \
    Dumper.cpp \
    SharedBufferStorageClient.cpp
