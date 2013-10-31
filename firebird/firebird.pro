include(../common.pri)

TARGET = firebird
TEMPLATE = lib
CONFIG += staticlib

SOURCES += firebird_driver.cpp

HEADERS += firebird_driver.h

INCLUDEPATH += ../cpp_db \
               ../tools
