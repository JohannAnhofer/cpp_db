include(../common.pri)

TARGET = postgres
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    postgres_connection.cpp \
    postgres_driver.cpp \
    postgres_exception.cpp

HEADERS += \
    postgres_connection.h \
    postgres_driver.h \
    postgres_exception.h

INCLUDEPATH += ../cpp_db \
               ../tools \
               "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/usr/include"


