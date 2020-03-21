TARGET = test_$${TARGET}
CONFIG(debug, debug|release): TARGET = $${TARGET}_debug

CONFIG += testcase cmdline

win32: CONFIG -= debug_and_release

CONFIG += c++11

ROOT_PATH = $$clean_path($$PWD/../..)

INCLUDEPATH += $$ROOT_PATH/sources
LIBS += -L$$shadowed($$ROOT_PATH)

QT = testlib

unix:!isEmpty(QMAKE_REL_RPATH_BASE): QMAKE_RPATHDIR += .

DESTDIR = $$shadowed($$ROOT_PATH)
