include(../default.pri)

TEMPLATE = lib
TARGET = cppipm

CONFIG   += staticlib
CONFIG   -= app_bundle qt

DEFINES += CPPIPM_LIBRARY

HEADERS += \
    global.h \
    core/Algorithm.h \
    core/Parameters.h \
    core/Problem.h \
    core/Status.h \
    utilities/mpsReader.h \
    algorithm/cppipm.h \
    core/GenericConvexOptimisationSolver.h

SOURCES += \
    algorithm/cppipm.cpp \
    utilities/mpsReader.cpp


