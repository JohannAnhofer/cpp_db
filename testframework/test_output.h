#ifndef TEST_TEST_OUTPUT_H
#define TEST_TEST_OUTPUT_H

#include <string>

namespace test
{

struct test_class_statistics;

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
    virtual void output_exception(const std::string &exception_text, const std::string &message, int line = 0, const char *file = nullptr) = 0;
    virtual void output_message(const std::string &message, int line = 0, const char *file = nullptr) = 0;
    virtual void output_expected_fail(const std::string &expected, const std::string &message, int line = 0, const char *file = nullptr) = 0;
};

}

#endif // TEST_TEST_OUTPUT_H
