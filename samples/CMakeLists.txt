
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(samples main.cpp samples.cpp)
target_link_libraries(samples PRIVATE
    tools
    sqlite
    firebird
    cpp_db
    test
    ${CMAKE_DL_LIBS}
    Threads::Threads)

if (APPLE)
    target_link_directories(samples PRIVATE lib_macosx_64)
endif()
