include(../common.pri)

TARGET = tools
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    cpp11_defines.h \
    key_value_pair.h \
    nullable.h \
    nullable_types.h \
    value_is_null.h \
    compiler_specific.h \
    coalesce.h \
    check_pointer.h \
    lock_or_throw.h

SOURCES += \
    value_is_null.cpp
