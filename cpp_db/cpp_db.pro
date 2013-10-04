include(../common.pri)

TARGET = cpp_db
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    connection.cpp \
    statement.cpp \
    record.cpp

HEADERS += \
    connection.h \
    statement.h \
    record.h \
    db_exception.h

INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db
