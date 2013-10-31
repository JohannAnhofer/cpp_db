
# dump OS info
win32:  message(Running under win32)
unix:   message(Running under unix/linux)
macx:   message(Running under Mac OS X)

# dump Compiler info
gcc:    message(Compiling with GCC)
msvc:   message(Compiling with MS VC)
mingw:  message(Compiling with MinGW)
clang:  message(Compiling with Clang)
llvm:   message(Compiling with LLVM)

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += tools \
           sqlite \
           firebird \
           cpp_db \
           test \
           main \

OTHER_FILES += todo.txt

cache()
