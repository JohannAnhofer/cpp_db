include(../common.pri)

TARGET = cppdb
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SOURCES += main.cpp \
    abstract_test.cpp \
    test_cpp_db.cpp \
    test_tools.cpp

HEADERS += \
    abstract_test.h \
    abstract_inl.h

# add include path and dependend to sqlite library
INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

win32 {
    mingw {
        # use sqlite, tools and cpp_db as static lib
        CONFIG(release, debug|release) {

            LIBS += \
                -L$$OUT_PWD/../cpp_db/release/ -lcpp_db \
                -L$$OUT_PWD/../sqlite/release/ -lsqlite \
                -L$$OUT_PWD/../tools/release/ -ltools

            PRE_TARGETDEPS += \
                $$OUT_PWD/../cpp_db/release/libcpp_db.a \
                $$OUT_PWD/../sqlite/release/libsqlite.a \
                $$OUT_PWD/../tools/release/libtools.a
        }

        CONFIG(debug, debug|release) {

            LIBS += \
                -L$$OUT_PWD/../cpp_db/debug/ -lcpp_db \
                -L$$OUT_PWD/../sqlite/debug/ -lsqlite \
                -L$$OUT_PWD/../tools/debug/ -ltools

            PRE_TARGETDEPS += \
                $$OUT_PWD/../cpp_db/debug/libcpp_db.a \
                $$OUT_PWD/../sqlite/debug/libsqlite.a \
                $$OUT_PWD/../tools/debug/libtools.a
        }
    }
}

unix {
    # add sqlite, tools and cpp_db to pre targetdeps
    PRE_TARGETDEPS += \
        $$OUT_PWD/../cpp_db/libcpp_db.a \
        $$OUT_PWD/../sqlite/libsqlite.a \
        $$OUT_PWD/../tools/libtools.a

    # add sqlite library to LIBS
    LIBS += \
        -L$$OUT_PWD/../cpp_db/ -lcpp_db \
        -L$$OUT_PWD/../sqlite/ -lsqlite \
        -L$$OUT_PWD/../tools/  -ltools \
        -ldl
}
