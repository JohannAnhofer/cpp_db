include(../common.pri)

TARGET = test
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    abstract_test.h \
    abstract_test.inl

SOURCES += \
    abstract_test.cpp
