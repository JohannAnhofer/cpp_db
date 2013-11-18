include(../common.pri)

TARGET = test
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    abstract_test.h \
    abstract_test.inl \
    junit_output.h \
    normal_output.h \
    test_app.h \
    test_class_statistics.h \
    test_output.h \
    tiny_output.h \
    windebug_output.h

SOURCES += \
    abstract_test.cpp \
    junit_output.cpp \
    normal_output.cpp \
    test_app.cpp \
    test_class_statistics.cpp \
    tiny_output.cpp \
    windebug_output.cpp

OTHER_FILES += \
    junit.xsd
