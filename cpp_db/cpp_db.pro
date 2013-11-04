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
    execute.h \
    statement.inl.h \
    driver.h \
    usings.h \
    connection_interface.h \
    statement_interface.h \
    parameters_interface.h \
    result_interface.h \
    transaction_interface.h \
    value.inl \
    type_of.h \
    user_password_authentication.h \
    no_authentication.h \
    authentication.h

INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db
