include(../common.pri)

TARGET = sqlite
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
#    shell.c \
    sqlite_connection.cpp \
    sqlite_driver.cpp \
    sqlite_parameters.cpp \
    sqlite_result.cpp \
    sqlite_statement.cpp \
    sqlite_transaction.cpp \
    sqlite3.c \
    sqlite_exception.cpp

HEADERS += \
    sqlite_connection.h \
    sqlite_driver.h \
    sqlite_parameters.h \
    sqlite_result.h \
    sqlite_statement.h \
    sqlite_transaction.h \
    sqlite3.h \
    sqlite3ext.h \
    sqlite_exception.h

INCLUDEPATH += ../cpp_db \
               ../tools
