include(../common.pri)

TARGET = sqlite
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
#    shell.c \
    sqlite3.c

HEADERS += \
    sqlite3ext.h \
    sqlite3.h
