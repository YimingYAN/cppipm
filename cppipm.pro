TEMPLATE = app
TARGET = cppipm

CONFIG += c++11
CONFIG += thread

QMAKE_CXXFLAGS += -O3

INCLUDEPATH += include \
               lib \
               lib/Eigen \
               lib/Eigen/src/

SOURCES += \
    src/cppipm.cpp \
    src/mpsReader.cpp \
    examples/test.cpp

HEADERS += include/*.h \
           lib/Eigen/* \
           lib/Eigen/BenchUtilities/BenchTimer.h

OTHER_FILES += \
    examples/*.QPS

win32:{
    PWD_WIN = $${PWD}
    DESTDIR_WIN = $${OUT_PWD}
    PWD_WIN ~= s,/,\\,g
    DESTDIR_WIN ~= s,/,\\,g

    copydata.commands = $(COPY_DIR) $$PWD_WIN\examples $$DESTDIR_WIN\debug
}
else: copydata.commands = $(COPY_DIR) $$PWD/examples $$DESTDIR/debug

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
