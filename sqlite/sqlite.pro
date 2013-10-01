#-------------------------------------------------
#
# Project created by QtCreator 2013-10-01T13:22:40
#
#-------------------------------------------------

QT       -= core gui

TARGET = sqlite
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
#    shell.c \
    sqlite3.c

HEADERS += \
    sqlite3ext.h \
    sqlite3.h
