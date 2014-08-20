TEMPLATE = app
TARGET = cppipm

CONFIG += c++11
CONFIG += thread

QMAKE_CXXFLAGS += -O3

INCLUDEPATH += include \
               lib \
               lib/Eigen \
               lib/Eigen/src/

SOURCES += test.cpp \
    src/cppipm.cpp \
    src/mpsReader.cpp

HEADERS += include/*.h \
           lib/Eigen/* \
           lib/Eigen/BenchUtilities/BenchTimer.h

OTHER_FILES += \
    examples/TESTPROB.QPS
