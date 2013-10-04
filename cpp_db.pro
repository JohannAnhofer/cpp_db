
# dump OS info
win32:  message(Running under win32)
unix:   message(Running under unix/linux)
macx:   message(Running under Mac OS X)

# dump Compiler info
g++:    message(Compiling with GCC)
msvc:   message(Compiling with MS VC)
mingw:  message(Compiling with MinGW)
clang:  message(Compiling with Clang)
llvm:   message(Compiling with LLVM)

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += sqlite \
           tools \
           cpp_db \
           main

cache()
