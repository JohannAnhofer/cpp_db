include(../common.pri)

TARGET = samples
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += main.cpp \
    samples.cpp

INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../firebird \
    $$PWD/../cpp_db \
    $$PWD/../test

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../firebird \
    $$PWD/../cpp_db \
    $$PWD/../test

win32 {
    mingw {
        # use sqlite, tools and cpp_db as static lib
        CONFIG(release, debug|release) {

            LIBS += \
                -L$$OUT_PWD/../cpp_db/release/ -lcpp_db \
                -L$$OUT_PWD/../sqlite/release/ -lsqlite \
                -L$$OUT_PWD/../firebird/release/ -lfirebird

            PRE_TARGETDEPS += \
                $$OUT_PWD/../cpp_db/release/libcpp_db.a \
                $$OUT_PWD/../sqlite/release/libsqlite.a \
                $$OUT_PWD/../firebird/release/libfirebird.a
        }

        CONFIG(debug, debug|release) {

            LIBS += \
                -L$$OUT_PWD/../cpp_db/debug/ -lcpp_db \
                -L$$OUT_PWD/../sqlite/debug/ -lsqlite \
                -L$$OUT_PWD/../firebird/debug/ -lfirebird

            PRE_TARGETDEPS += \
                $$OUT_PWD/../cpp_db/debug/libcpp_db.a \
                $$OUT_PWD/../sqlite/debug/libsqlite.a \
                $$OUT_PWD/../firebird/debug/libfirebird.a
        }
    }
}

unix {
    # add sqlite, tools and cpp_db to pre targetdeps
    PRE_TARGETDEPS += \
        $$OUT_PWD/../cpp_db/libcpp_db.a \
        $$OUT_PWD/../sqlite/libsqlite.a \
        $$OUT_PWD/../firebird/libfirebird.a

    # add sqlite library to LIBS
    LIBS += \
        -L$$OUT_PWD/../cpp_db/ -lcpp_db \
        -L$$OUT_PWD/../sqlite/ -lsqlite \
        -L$$OUT_PWD/../firebird/ -lfirebird
}

win32 {
    LIBS += $$PWD/../firebird/lib_win_32/fbclient_ms.lib
} else: macx {
    LIBS += -L$$PWD/../firebird/lib_macosx_64/ -lfbclient
} else: unix {

!contains(QMAKE_HOST.arch, x86_64): LIBS += -L$$PWD/../firebird/lib_linux_32/
contains(QMAKE_HOST.arch, x86_64): LIBS += -L$$PWD/../firebird/lib_linux_64/

    LIBS += -lfbclient \
            -ldl \
            -lpthread
}

