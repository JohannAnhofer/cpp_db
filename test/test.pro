include(../common.pri)

TARGET = test
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    abstract_test.h \
    abstract_test.inl \
    test_app.h \
    test_output.h \
    tiny_output.h \
    normal_output.h \
    junit_output.h

SOURCES += \
    abstract_test.cpp \
    test_app.cpp \
    tiny_output.cpp \
    normal_output.cpp \
    junit_output.cpp

OTHER_FILES += \
    junit.xsd
