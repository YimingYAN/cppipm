include(../default.pri)
#include(testrpath.pri)
isEmpty(TEMPLATE):TEMPLATE=app
QT += testlib
CONFIG += qt warn_on console depend_includepath testcase
CONFIG -= app_bundle

DEFINES -= QT_NO_CAST_FROM_ASCII
# prefix test binary with tst_
!contains(TARGET, ^tst_.*):TARGET = $$join(TARGET,,"tst_")

win32:CONFIG(release, debug|release): LIBS+= -L$$OUT_PWD/../../bin/release -lcppipm
else:win32:CONFIG(debug, debug|release): LIBS+= -L$$OUT_PWD/../../bin/debug -lcppipm
else: LIBS+= -L$$OUT_PWD/../../bin -lcppipm
