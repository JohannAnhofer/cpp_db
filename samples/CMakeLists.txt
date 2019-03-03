
add_executable(samples main.cpp samples.cpp)
target_link_libraries(samples PRIVATE
    tools
    sqlite
    firebird
    cpp_db
    test
    fbclient
    dl
    pthread)
