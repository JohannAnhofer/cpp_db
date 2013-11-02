include(../common.pri)

TARGET = cppdb
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

HEADERS += \
    test_cpp_db_class.h \
    test_tools_class.h \
    test_test_class.h \
    test_firebird_class.h

SOURCES += main.cpp \
    test_tools_class.cpp \
    test_cpp_db_class.cpp \
    test_test_class.cpp \
    test_firebird_class.cpp

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
                -L$$OUT_PWD/../firebird/release/ -lfirebird \
                -L$$OUT_PWD/../test/release/ -ltest \
                -L$$OUT_PWD/../tools/release/ -ltools \
                $$PWD/../firebird/lib_win_32/fbclient_ms.lib

            PRE_TARGETDEPS += \
                $$OUT_PWD/../cpp_db/release/libcpp_db.a \
                $$OUT_PWD/../sqlite/release/libsqlite.a \
                $$OUT_PWD/../firebird/release/libfirebird.a \
                $$OUT_PWD/../test/release/libtest.a \
                $$OUT_PWD/../tools/release/libtools.a
        }

        CONFIG(debug, debug|release) {

            LIBS += \
                -L$$OUT_PWD/../cpp_db/debug/ -lcpp_db \
                -L$$OUT_PWD/../sqlite/debug/ -lsqlite \
                -L$$OUT_PWD/../firebird/debug/ -lfirebird \
                -L$$OUT_PWD/../test/debug/ -ltest \
                -L$$OUT_PWD/../tools/debug/ -ltools \
                $$PWD/../firebird/lib_win_32/fbclient_ms.lib

            PRE_TARGETDEPS += \
                $$OUT_PWD/../cpp_db/debug/libcpp_db.a \
                $$OUT_PWD/../sqlite/debug/libsqlite.a \
                $$OUT_PWD/../firebird/debug/libfirebird.a \
                $$OUT_PWD/../test/debug/libtest.a \
                $$OUT_PWD/../tools/debug/libtools.a
        }
    }
}

unix {
    # add sqlite, tools and cpp_db to pre targetdeps
    PRE_TARGETDEPS += \
        $$OUT_PWD/../cpp_db/libcpp_db.a \
        $$OUT_PWD/../sqlite/libsqlite.a \
        $$OUT_PWD/../firebird/libfirebird.a \
        $$OUT_PWD/../test/libtest.a \
        $$OUT_PWD/../tools/libtools.a

    # add sqlite library to LIBS
    LIBS += \
        -L$$OUT_PWD/../cpp_db/ -lcpp_db \
        -L$$OUT_PWD/../sqlite/ -lsqlite \
        -L$$OUT_PWD/../firebird/ -lfirebird \
        -L$$OUT_PWD/../test/ -ltest \
        -L$$OUT_PWD/../tools/  -ltools \
}

macx {
    LIBS += -L$$PWD/../firebird/lib_macosx_64/ -lfbclient
} else: unix {
   LIBS += -L$$PWD/../firebird/lib_linux_64/ -lfbclient \
            -ldl
}

