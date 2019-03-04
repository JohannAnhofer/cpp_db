
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(samples main.cpp samples.cpp)
target_link_libraries(samples PRIVATE
    tools
    sqlite
    cpp_db
    ${CMAKE_DL_LIBS}
    Threads::Threads)

