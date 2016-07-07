include(../default.pri)

TEMPLATE = lib
TARGET = cppipm

CONFIG   += staticlib
CONFIG   -= qt

DEFINES += CPPIPM_LIBRARY

HEADERS += \
    global.h \
    core/Algorithm.h \
    core/Parameters.h \
    core/Problem.h \
    core/Status.h \
    utilities/mpsReader.h \
    algorithm/cppipm.h \
    core/GenericConvexOptimisationSolver.h \
    core/Vector.h \
    core/Matrix.h \
    core/Operations.h

SOURCES += \
    algorithm/cppipm.cpp \
    utilities/mpsReader.cpp \
    core/Vector.cpp \
    core/Matrix.cpp \
    core/Operations.cpp


