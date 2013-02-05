ROOT_PATH = ../../..
TEMPLATE = app
TARGET = sharbuf_cli
DESTDIR = $${ROOT_PATH}/bin

OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += \
    -llog4cxx

LIB_PATH = $${ROOT_PATH}/src/lib

include($${LIB_PATH}/SharedBuffer.pri)
INCLUDEPATH += $${LIB_PATH}
DEPENDPATH += $${LIB_PATH}

HEADERS += \
    _SharedBufferStorageClient.h \
    _Pusher.h \
    _Dumper.h

SOURCES += \
    main.cpp \
    _SharedBufferStorageClient.cpp \
    _Pusher.cpp \
    _Dumper.cpp
