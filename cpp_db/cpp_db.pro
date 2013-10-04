QT       += core
QT       -= gui

TARGET = cppdb
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp \
    connection.cpp \
    statement.cpp \
    record.cpp \
    test_cpp_db.cpp

HEADERS += \
    nullable.h \
    value_is_null.h \
    nullable_types.h \
    key_value_pair.h \
    coalesce.h \
    null.h \
    test.h \
    connection.h \
    statement.h \
    record.h \
    cpp11_defines.h \
    db_exception.h

# dump OS info
win32:  message(Running under win32)
unix:   message(Running under unix/linux)
macx:   message(Running under Mac OS X)

# dump Compiler info
g++:    message(Compiling with GCC)
msvc:   message(Compiling with MS VC)
mingw:  message(Compiling with MinGW)
clang:  message(Compiling with Clang)
llvm:   message(Compiling with LLVM)

# use sqlite static lib

# add include path and dependend to sqlite library
INCLUDEPATH += $$PWD/../sqlite
DEPENDPATH += $$PWD/../sqlite

# add sqlite library to LIBS
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../sqlite/release/ -lsqlite
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../sqlite/debug/ -lsqlite
else:unix: LIBS += -L$$OUT_PWD/../sqlite/ -lsqlite

# add sqlite to pre targetdeps
win32 {
    mingw {
        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/release/libsqlite.a
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/debug/libsqlite.a
    }
    msvc {
        CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/release/sqlite.lib
        CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../sqlite/debug/sqlite.lib
    }
}
unix: PRE_TARGETDEPS += $$OUT_PWD/../sqlite/libsqlite.a

