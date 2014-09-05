CONFIG += c++11
CONFIG += qt warn_on console depend_includepath testcase

CONFIG -= app_bundle
CONFIG -= qt

DEFINES -= QT_NO_CAST_FROM_ASCII

QMAKE_CXXFLAGS += -O3
