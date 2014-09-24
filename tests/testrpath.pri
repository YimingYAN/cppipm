linux-* {

} else:macx {
    QMAKE_LFLAGS+=-Wl,-rpath,\"$$OUT_PWD/../../src\"
}
