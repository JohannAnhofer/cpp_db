include(../common.pri)

TARGET = postgres
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    postgres_connection.cpp \
    postgres_driver.cpp \
    postgres_exception.cpp \
    postgres_transaction.cpp

HEADERS += \
    postgres_connection.h \
    postgres_driver.h \
    postgres_exception.h \
    postgres_transaction.h

INCLUDEPATH += ../cpp_db \
               ../tools

INCLUDEPATH += $$(POSTGRES_INCLUDE_PATH)


