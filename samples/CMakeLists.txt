
add_executable(samples main.cpp samples.cpp)
target_link_libraries(samples PRIVATE
    tools
    sqlite
    cpp_db
)

