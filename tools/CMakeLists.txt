
add_library(tools STATIC
    check_pointer.h
    coalesce.h
    key_value_pair.h
    lock_or_throw.h
    nullable_types.h
    tools.cpp
    )

target_include_directories(tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
