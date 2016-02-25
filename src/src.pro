include(../default.pri)

TEMPLATE = lib
TARGET = cppipm

CONFIG   += console
CONFIG   += c++11
CONFIG   += thread
CONFIG   -= app_bundle
CONFIG   -= qt

DEFINES += CPPIPM_LIBRARY

win32-msvc* {
    QMAKE_CXXFLAGS += /O2
}

mac {
    QMAKE_CXXFLAGS += -O3
}

linux-g++ {
    QMAKE_CXXFLAGS += -O3
}

SOURCES += *.cpp
HEADERS += *.h
HEADERS += cppipm
