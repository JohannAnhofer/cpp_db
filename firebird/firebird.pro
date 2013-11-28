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
    xsqlvar.cpp

HEADERS += \
    firebird_connection.h \
    firebird_driver.h \
    firebird_parameters.h \
    firebird_result.h \
    firebird_statement.h \
    firebird_transaction.h \
    isc_status.h \
    xsqlda.h \
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
!contains(QMAKE_HOST.arch, x86_64): INCLUDEPATH += include_linux_32
contains(QMAKE_HOST.arch, x86_64): INCLUDEPATH += include_linux_64
}
