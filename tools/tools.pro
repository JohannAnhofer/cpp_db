include(../common.pri)

TARGET = tools
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    coalesce.h \
    cpp11_defines.h \
    nullable.h \
    nullable_types.h \
    value_is_null.h \
    null.h

SOURCES += \
    value_is_null.cpp
