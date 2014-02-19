include(../common.pri)

TARGET = cpp_db
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    connection.cpp \
    execute.cpp \
    parameters.cpp \
    result.cpp \
    statement.cpp \
    transaction.cpp \
    driver_factory.cpp \
    type_mismatch.cpp \
    driver_registry.cpp

HEADERS += \
    authentication.h \
    connection.h \
    connection_interface.h \
    db_exception.h \
    execute.h \
    no_authentication.h \
    null.h \
    parameter.h \
    parameters.h \
    parameters_interface.h \
    result.h \
    result_interface.h \
    statement.h \
    statement.inl \
    statement_interface.h \
    transaction.h \
    transaction_interface.h \
    transaction_scope.h \
    type_of.h \
    user_password_authentication.h \
    usings.h \
    value.h \
    value.inl \
    value_of.h \
    value_is_null.h \
    driver_interface.h \
    driver_factory.h \
    type_mismatch.h \
    driver_registry.h

INCLUDEPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db

DEPENDPATH += \
    $$PWD/../tools \
    $$PWD/../sqlite \
    $$PWD/../cpp_db
