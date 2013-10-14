include(../common.pri)

TARGET = tools
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    coalesce.h \
    cpp11_defines.h \
    key_value_pair.h \
    null.h \
    nullable.h \
    nullable_types.h \
    value_is_null.h \
    compiler_specific.h

SOURCES += \
    value_is_null.cpp
