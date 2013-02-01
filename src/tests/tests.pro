ROOT_PATH = ../..
TEMPLATE = app
TARGET = tests
DESTDIR = $${ROOT_PATH}/bin

APP_PATH = $${ROOT_PATH}/src/app
include($${APP_PATH}/SharedBuffer.pri)
INCLUDEPATH += $${APP_PATH}
DEPENDPATH += $${APP_PATH}
LIBS += -lgtest

OBJECTS_DIR = .obj
MOC_DIR = .moc

SOURCES += \
    _Main.cpp \
    test_BufferManager.cpp

HEADERS += \
    Global.h
