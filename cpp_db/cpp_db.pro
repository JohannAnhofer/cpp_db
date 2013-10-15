include(../common.pri)

TARGET = cpp_db
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    connection.cpp \
    parameters.cpp \
    result.cpp \
    statement.cpp \
    transaction.cpp

HEADERS += \
    connection.h \
    db_exception.h \
    parameter.h \
    parameters.h \
    result.h \
    statement.h \
    transaction.h \
    value.h \
    null.h \
    coalesce.h \
    execute.h

INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db
