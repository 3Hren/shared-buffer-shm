QMAKE_CXXFLAGS += -std=c++11

LIBS += \
    -llog4cxx \
    -lboost_timer

HEADERS += \
    BufferManager.h \
    SharedBufferGlobal.h \
    Server.h \
    Pusher.h \
    Dumper.h

SOURCES += \
    BufferManager.cpp \
    Server.cpp \
    Pusher.cpp \
    Dumper.cpp
