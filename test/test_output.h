#ifndef TEST_TEST_OUTPUT_H
#define TEST_TEST_OUTPUT_H

#include <string>

namespace test
{

struct test_class_statistics
{
    test_class_statistics()
        : function_count(0)
        , success_count(0)
        , fail_count(0)
        , exception_count(0)
        , expected_exception_count(0)
    {
    }


    test_class_statistics &operator+=(const test_class_statistics &right)
    {
        function_count+=right.function_count;
        success_count+=right.success_count;
        fail_count+=right.fail_count;
        exception_count+=right.exception_count;
        expected_exception_count+=right.expected_exception_count;
        return *this;
    }

    int function_count;
    int success_count;
    int fail_count;
    int exception_count;
    int expected_exception_count;
};

struct test_output
{
    virtual ~test_output() {}

    virtual void start() = 0;
    virtual void start_class(const std::string &class_name) = 0;
    virtual void start_function(const std::string &class_name, const std::string &function_name) = 0;
    virtual void end_function(const std::string &class_name, const std::string &function_name) = 0;
    virtual void end_class(const std::string &class_name, const test_class_statistics &stats) = 0;
    virtual void end(int class_count, const test_class_statistics &stats) = 0;

    virtual void output_success(const std::string &message, int line = 0, const char *file = nullptr) = 0;
    virtual void output_failure(const std::string &message, int line = 0, const char *file = nullptr) = 0;
    virtual void output_exception(const std::string &message, int line = 0, const char *file = nullptr) = 0;
    virtual void output_message(const std::string &message, int line = 0, const char *file = nullptr) = 0;
};

}

#endif // TEST_TEST_OUTPUT_H
