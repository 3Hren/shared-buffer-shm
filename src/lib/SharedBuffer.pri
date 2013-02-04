QMAKE_CXXFLAGS += -std=c++11

LIBS += \
    -llog4cxx \
    -lboost_timer

HEADERS += \
    LowLevelBufferManager.h \
    SharedBufferGlobal.h \
    SharedBufferStorageServer.h \
    Pusher.h \
    Dumper.h \
    SharedBufferStorageClient.h \
    HighLevelBufferManager.h

SOURCES += \
    SharedBufferStorageServer.cpp \
    Pusher.cpp \
    Dumper.cpp \
    SharedBufferStorageClient.cpp \
    LowLevelBufferManager.cpp \
    HighLevelBufferManager.cpp
