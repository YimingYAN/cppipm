include(../default.pri)

TEMPLATE = lib
TARGET = cppipm

CONFIG   += console
CONFIG   += c++11
CONFIG   += thread
CONFIG   -= app_bundle
CONFIG   -= qt

win32:CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../bin/release
else:win32:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../bin/debug
else: DESTDIR = $$OUT_PWD/../bin

message($$DESTDIR)

DEFINES += CPPIPM_LIBRARY

SOURCES += *.cpp
HEADERS += *.h
HEADERS += cppipm
