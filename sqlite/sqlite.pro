include(../common.pri)

TARGET = sqlite
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
#    shell.c \
    sqlite3.c \
    sqlite_driver.cpp \
    sqlite_connection.cpp \
    sqlite_statement.cpp \
    sqlite_transaction.cpp \
    sqlite_parameters.cpp \
    sqlite_result.cpp

HEADERS += \
    sqlite3ext.h \
    sqlite3.h \
    sqlite_driver.h \
    sqlite_connection.h \
    sqlite_statement.h \
    sqlite_transaction.h \
    sqlite_parameters.h \
    sqlite_result.h

INCLUDEPATH += ../cpp_db \
               ../tools
