include(../common.pri)

TARGET = odbc
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    odbc_driver.cpp \
    odbc_connection.cpp \
    odbc_statement.cpp \
    odbc_parameters.cpp \
    odbc_result.cpp \
    odbc_transaction.cpp

HEADERS += \
    odbc_driver.h \
    odbc_connection.h \
    odbc_statement.h \
    odbc_parameters.h \
    odbc_result.h \
    odbc_transaction.h

INCLUDEPATH += ../cpp_db \
               ../tools \
