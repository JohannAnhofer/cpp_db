
add_library(testframework STATIC
    abstract_test.cpp
    abstract_test.h
    abstract_test.inl
    junit_output.cpp
    junit_output.h
    normal_output.cpp
    normal_output.h
    test_app.cpp
    test_app.h
    test_class_statistics.cpp
    test_class_statistics.h
    test_output.h
    tiny_output.cpp
    tiny_output.h
    windebug_output.cpp
    windebug_output.h
)

add_custom_target(other_files junit.xsd)
target_include_directories(testframework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
