ROOT_PATH = ../..
TEMPLATE = app
TARGET = app
DESTDIR = $${ROOT_PATH}/bin

OBJECTS_DIR = .obj
MOC_DIR = .moc

include(SharedBuffer.pri)

SOURCES += \
    main.cpp
