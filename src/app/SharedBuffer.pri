QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    BufferManager.h \
    ../app/SharedBufferGlobal.h \
    ../app/Server.h \
    ../app/Pusher.h \
    ../app/Dumper.h

SOURCES += \
    BufferManager.cpp \
    ../app/Server.cpp \
    ../app/Pusher.cpp \
    ../app/Dumper.cpp
