TEMPLATE = lib

CONFIG(debug, debug|release): TARGET = $${TARGET}_debug

win32: CONFIG -= debug_and_release

CONFIG += c++11

ROOT_PATH = $$clean_path($$PWD/../..)

INCLUDEPATH += $$ROOT_PATH/sources
LIBS += -L$$shadowed($$ROOT_PATH)

QT = core

DESTDIR = $$shadowed($$ROOT_PATH)
