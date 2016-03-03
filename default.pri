INCLUDEPATH +=  \
    $$PWD/src \
    $$PWD/libs \
    $$PWD/libs/Eigen

CONFIG += console c++11 thread

# Compiler flags
win32-msvc* {
    QMAKE_CXXFLAGS += /O2
}

mac {
    QMAKE_CXXFLAGS += -O2
}

linux-g++ {
    QMAKE_CXXFLAGS += -O2
}
