include(../default.pri)

TEMPLATE = lib
TARGET = cppipm

CONFIG   += staticlib
CONFIG   -= app_bundle qt

DEFINES += CPPIPM_LIBRARY

HEADERS += \
    global.h \
    Core/Algorithm.h \
    Core/AbstractConvexOptimisationSolver.h \
    Core/Parameters.h \
    Core/Problem.h \
    Core/Status.h \
    Utilities/mpsReader.h \
    Algorithm/cppipm.h

SOURCES += \
    Algorithm/cppipm.cpp \
    Utilities/mpsReader.cpp


