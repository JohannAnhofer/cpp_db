include(../common.pri)

TARGET = test
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    abstract_test.h \
    abstract_test.inl \
    test_app.h

SOURCES += \
    abstract_test.cpp \
    test_app.cpp
