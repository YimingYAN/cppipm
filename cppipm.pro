TEMPLATE = app
TARGET = test

CONFIG += c++11
CONFIG += thread
mac: CONFIG-=app_bundle

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

win32:{
    PWD_WIN = $${PWD}
    DESTDIR_WIN = $${OUT_PWD}
    PWD_WIN ~= s,/,\\,g
    DESTDIR_WIN ~= s,/,\\,g

    CopyTestProb.commands = $(COPY_DIR) $$PWD_WIN\examples $$DESTDIR_WIN\debug
}
else: CopyTestProb.commands = $(COPY_DIR) $$PWD/examples/* $${OUT_PWD}

first.depends = $(first) CopyTestProb
export(first.depends)
export(CopyTestProb.commands)
QMAKE_EXTRA_TARGETS += first CopyTestProb
