find_package(boost)
project(tools)
add_library(tools STATIC
    cpp11_defines.h
    check_pointer.h
    coalesce.h
    compiler_specific.h
    cpp11_defines.h
    key_value_pair.h
    lock_or_throw.h
    nullable.h
    nullable_types.h
    tools.cpp
    )

