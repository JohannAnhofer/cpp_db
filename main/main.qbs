import qbs 1.0

Application {
    name: "cpp_db_tests"
    files: { "main.cpp",
             "test_test_class.cpp",
             "test_tools_class.cpp",
             "test_cpp_db_class.cpp",
             "test_firebird_class.cpp"
             "test_test_class.h",
             "test_tools_class.h",
             "test_cpp_db_class.h",
             "test_firebird_class.h"
            }
    Depends { name: "cpp" }
}
