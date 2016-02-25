include(../default.pri)
include(testrpath.pri)

isEmpty(TEMPLATE):TEMPLATE=app
QT += testlib
CONFIG += qt warn_on console depend_includepath testcase
CONFIG -= app_bundle

win32-msvc* {
    QMAKE_CXXFLAGS += /O2
}

mac {
    QMAKE_CXXFLAGS += -O3
}

linux-g++ {
    QMAKE_CXXFLAGS += -O3
}

DEFINES -= QT_NO_CAST_FROM_ASCII
# prefix test binary with tst_
!contains(TARGET, ^tst_.*):TARGET = $$join(TARGET,,"tst_")

win32:CONFIG(release, debug|release): LIBS+= -L$$OUT_PWD/../../src/release -lcppipm
else:win32:CONFIG(debug, debug|release): LIBS+= -L$$OUT_PWD/../../src/debug -lcppipm
else: LIBS+= -L$$OUT_PWD/../../src -lcppipm

OTHER_FILES += \
    ../testrpath.pri
