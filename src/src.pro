include(../default.pri)

TEMPLATE = lib
TARGET = cppipm

CONFIG   += console
CONFIG   += c++11
CONFIG   += thread
CONFIG   -= app_bundle
CONFIG   -= qt

DEFINES += CPPIPM_LIBRARY

SOURCES += *.cpp
HEADERS += *.h
HEADERS += cppipm
