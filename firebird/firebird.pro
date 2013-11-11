include(../common.pri)

TARGET = firebird
TEMPLATE = lib
CONFIG += staticlib

SOURCES += firebird_driver.cpp \
    firebird_connection.cpp \
    firebird_statement.cpp \
    firebird_transaction.cpp \
    firebird_parameters.cpp \
    xsqlda.cpp \
    firebird_result.cpp \
    xsqlvar.cpp

HEADERS += firebird_driver.h \
    firebird_connection.h \
    firebird_statement.h \
    firebird_transaction.h \
    isc_status.h \
    firebird_parameters.h \
    xsqlda.h \
    firebird_result.h \
    xsqlvar.h

INCLUDEPATH += ../cpp_db \
               ../tools \

win32 {
INCLUDEPATH += include_win_32
LIBS += $$PWD/lib_win_32/fbclient_ms.lib
}

macx {
INCLUDEPATH += include_macosx_64
}

linux-g++ {
INCLUDEPATH += include_linux_64
}
