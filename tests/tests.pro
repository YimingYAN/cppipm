include(../default.pri)

TEMPLATE  = app
TARGET    = test

CONFIG   += console
CONFIG   += c++11
CONFIG   += thread
CONFIG   -= app_bundle
CONFIG   -= qt

QMAKE_CXXFLAGS += -O3

LIBS += -L../src -lcppipm
