include(../common.pri)

TARGET = cpp_db
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    connection.cpp \
    statement.cpp \
    record.cpp \
    transaction.cpp \
    parameters.cpp

HEADERS += \
    connection.h \
    statement.h \
    record.h \
    db_exception.h \
    transaction_scope.h \
    transaction.h \
    parameter.h \
    parameters.h \
    value.h

INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db
