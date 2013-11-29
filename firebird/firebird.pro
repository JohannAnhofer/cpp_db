include(../common.pri)

TARGET = firebird
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    firebird_connection.cpp \
    firebird_driver.cpp \
    firebird_parameters.cpp \
    firebird_result.cpp \
    firebird_statement.cpp \
    firebird_transaction.cpp \
    xsqlda.cpp \
    xsqlvar.cpp \
    firebird_exception.cpp

HEADERS += \
    firebird_connection.h \
    firebird_driver.h \
    firebird_parameters.h \
    firebird_result.h \
    firebird_statement.h \
    firebird_transaction.h \
    isc_status.h \
    xsqlda.h \
    xsqlvar.h \
    firebird_exception.h

INCLUDEPATH += ../cpp_db \
               ../tools \
               include

win32 : LIBS += $$PWD/lib_win_32/fbclient_ms.lib
