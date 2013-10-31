include(../common.pri)

TARGET = firebird
TEMPLATE = lib
CONFIG += staticlib

SOURCES += firebird_driver.cpp \
    firebird_connection.cpp

HEADERS += firebird_driver.h \
    firebird_connection.h

INCLUDEPATH += ../cpp_db \
               ../tools \
               include

LIBS += $$PWD/lib/fbclient_ms.lib
