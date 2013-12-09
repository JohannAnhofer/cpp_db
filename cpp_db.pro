
# dump OS info
win32:  message(Running under win32)
win64:  message(Running under win64)
unix: {
macx: {
contains(QMAKE_HOST.arch, x86_64): message(Running under 64-bit Mac OS X)
!contains(QMAKE_HOST.arch, x86_64): message(Running under 32-bit Mac OS X)
} else {
contains(QMAKE_HOST.arch, x86_64): message(Running under 64-bit unix/linux)
!contains(QMAKE_HOST.arch, x86_64): message(Running under 32-bit unix/linux)
}
}

# dump Compiler info
gcc: clang: {
    llvm: message(Compiling with Clang/LLVM) else: message(Compiling with Clang)
} else: {
    mingw: {
        *-g++*: message(Compiling with MinGW/GCC/G++) else: message(Compiling with MinGW/GCC)
    } else: {
        *-g++*: message(Compiling with GCC/G++) else: message(Compiling with GCC)
    }
}
msvc:   message(Compiling with MS VC)

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    tools \
    sqlite \
    firebird \
    odbc \
    cpp_db \
    test \
    main \
    samples

OTHER_FILES += todo.txt

cache()
