#include "normal_output.h"
#include "test_class_statistics.h"

#include <stdexcept>
#include <ostream>

namespace test
{

normal_output::normal_output(std::ostream *output_stream)
    : output(output_stream)
    , separator(80, '=')
{
    if (!output)
        throw std::runtime_error("No outputstream");
}

void normal_output::start()
{
    *output << "\n";
}

void normal_output::start_class(const std::string &class_name)
{
    *output << separator << "\n"
            << "Starting " << class_name << "...\n"
            << separator << "\n";
}

void normal_output::start_function(const std::string &, const std::string &function_name)
{
    *output << separator << "\n"
            << "Run function: " << function_name << "\n"
            << separator << "\n";
}

void normal_output::end_function(const std::string &, const std::string &function_name)
{
    *output << separator << "\n"
            << "End function: " << function_name << "\n"
            << separator << "\n";
}

void normal_output::end_class(const std::string &class_name, const test_class_statistics &stats)
{
    *output << separator << "\n"
            << "Summary for " << class_name << ":\n"
            << stats.function_count << " test functions called\n"
            << stats.success_count + stats.fail_count << " test cases executed\n"
            << stats.fail_count << " test cases failed\n"
            << stats.success_count << " test cases succeded\n"
            << stats.exception_count << " exceptions occured and "
            << stats.expected_exception_count << " where expected\n"
            << separator << "\n";
}

void normal_output::end(int class_count, const test_class_statistics &stats)
{
    *output << "Summary:\n"
            << class_count << " class" << (class_count > 1 ? "es" : "") << " executed\n"
            << stats.function_count << " test functions called\n"
            << stats.success_count + stats.fail_count << " test cases executed\n"
            << stats.fail_count << " test cases failed\n"
            << stats.success_count << " test cases succeded\n"
            << stats.exception_count << " exceptions occured and "
            << stats.expected_exception_count << " where expected\n"
            << separator << "\n";
}


void normal_output::output_success(const std::string &message, int, const char *)
{
    *output << "ok : " << message << std::endl;
}

void normal_output::output_failure(const std::string &message, int line, const char *file)
{
    *output << "NOK : " << message;
    if (file)
        *output << "\n   Loc: [" << file << "(" << line << ")]";
    *output << std::endl;
}

void normal_output::output_exception(const std::string &message, const std::string &exception_message, int line, const char *file)
{
    *output << "EXCEPTION : " << exception_message << "\n   " << message;
    if (file)
        *output << "\n   Loc: [" << file << "(" << line << ")]";
    *output << std::endl;
}

void normal_output::output_message(const std::string &message, int, const char *)
{
    *output << message << std::endl;
}

void normal_output::output_expected_fail(const std::string &expected, const std::string &message, int line, const char *file)
{
    *output << "EXPECTED : " << expected << "\n   " << message;
    if (file)
        *output << "\n   Loc: [" << file << "(" << line << ")]";
    *output << std::endl;
}

} // namespace test
