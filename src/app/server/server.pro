ROOT_PATH = ../../..
TEMPLATE = app
TARGET = server
DESTDIR = $${ROOT_PATH}/bin

OBJECTS_DIR = .obj
MOC_DIR = .moc

LIB_PATH = $${ROOT_PATH}/src/lib

include($${LIB_PATH}/SharedBuffer.pri)
INCLUDEPATH += $${LIB_PATH}
DEPENDPATH += $${LIB_PATH}

SOURCES += \
    main.cpp
