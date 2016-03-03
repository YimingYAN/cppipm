include(../default.pri)
TEMPLATE=app

CONFIG   -= app_bundle qt

# Destination
win32:CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/../bin/release
else:win32:CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../bin/debug
else: DESTDIR = $$OUT_PWD/../bin

# Libs
win32:CONFIG(release, debug|release): LIBS+= -L$$OUT_PWD/../bin/release -lcppipm
else:win32:CONFIG(debug, debug|release): LIBS+= -L$$OUT_PWD/../bin/debug -lcppipm
else: LIBS+= -L$$OUT_PWD/../bin -lcppipm

SOURCES += \
    main.cpp
