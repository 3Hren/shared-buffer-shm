ROOT_PATH = ../../..
TEMPLATE = app
TARGET = client
DESTDIR = $${ROOT_PATH}/bin

OBJECTS_DIR = .obj
MOC_DIR = .moc

LIBS += \
    -llog4cxx \
#    -lboost_program_options \
#    -lboost_filesystem \
#    -lboost_regex

LIB_PATH = $${ROOT_PATH}/src/lib

include($${LIB_PATH}/SharedBuffer.pri)
INCLUDEPATH += $${LIB_PATH}
DEPENDPATH += $${LIB_PATH}

SOURCES += \
    main.cpp
