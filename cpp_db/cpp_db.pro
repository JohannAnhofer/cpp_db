include(../common.pri)

TARGET = cpp_db
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    connection.cpp \
    parameters.cpp \
    record.cpp \
    statement.cpp \
    transaction.cpp

HEADERS += \
    connection.h \
    db_exception.h \
    parameter.h \
    parameters.h \
    record.h \
    statement.h \
    transaction.h \
    transaction_scope.h \
    value.h \
    null.h \
    coalesce.h

INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db
