ROOT_PATH = ../..
TEMPLATE = lib
TARGET = $$qtLibraryTarget(sharbuf)
DESTDIR = $${ROOT_PATH}/lib

VERSION = 0.2.0

OBJECTS_DIR = .obj
MOC_DIR = .moc

include(SharedBuffer.pri)
