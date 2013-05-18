ROOT_PATH = ../../..
TEMPLATE = app
TARGET = sharbuf_srv
DESTDIR = $${ROOT_PATH}/bin

OBJECTS_DIR = .obj
MOC_DIR = .moc

QT -= gui

LIB_PATH = $${ROOT_PATH}/src/lib
UTILS_PATH = $${ROOT_PATH}/src/utils

include($${LIB_PATH}/SharedBuffer.pri)
include($${UTILS_PATH}/utils.pri)

INCLUDEPATH += \
    $${LIB_PATH} \
    $${UTILS_PATH}

DEPENDPATH += \
    $${LIB_PATH} \
    $${UTILS_PATH}

LIBS += \
    -llog4cxx \
    -lboost_program_options \
    -lboost_system \
    -lboost_filesystem \
    -lboost_regex

SOURCES += \
    main.cpp \
    SharedBufferOptionParser.cpp

HEADERS += \
    SharedBufferOptionParser.h
