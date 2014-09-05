include(../default.pri)

TEMPLATE = lib
TARGET = cppipm

CONFIG   += console
CONFIG   += c++11
CONFIG   += thread
CONFIG   -= app_bundle
CONFIG   -= qt

DEFINES += CPPIPM_LIBRARY

QMAKE_CXXFLAGS += -O3

SOURCES += *.cpp
HEADERS += *.h

