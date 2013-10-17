include(../common.pri)

TARGET = sqlite
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
#    shell.c \
    sqlite3.c \
    sqlite_driver.cpp

HEADERS += \
    sqlite3ext.h \
    sqlite3.h \
    sqlite_driver.h

INCLUDEPATH += ../cpp_db \
               ../tools
