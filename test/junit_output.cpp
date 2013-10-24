#include "junit_output.h"
#include "test_class_statistics.h"

#include <stdexcept>
#include <ostream>

namespace test
{

junit_output::junit_output(std::ostream *output_stream)
    : output(output_stream)
{
    if (!output)
        throw std::runtime_error("No outputstream");
}

void junit_output::start()
{
}

void junit_output::start_class(const std::string &class_name)
{
    (void)class_name;
}

void junit_output::start_function(const std::string &class_name, const std::string &function_name)
{
    (void)class_name;
    (void)function_name;
}

void junit_output::end_function(const std::string &class_name, const std::string &function_name)
{
    (void)class_name;
    (void)function_name;
}

void junit_output::end_class(const std::string &class_name, const test_class_statistics &stats)
{
    (void)class_name;
    (void)stats;
}

void junit_output::end(int class_count, const test_class_statistics &stats)
{
    (void)class_count;
    (void)stats;
}


void junit_output::output_success(const std::string &message, int line, const char *file)
{
    (void)line;
    (void)file;
    (void)message;
}

void junit_output::output_failure(const std::string &message, int line, const char *file)
{
    (void)line;
    (void)file;
    (void)message;
}

void junit_output::output_exception(const std::string &message, int line, const char *file)
{
    (void)line;
    (void)file;
    (void)message;
}

void junit_output::output_message(const std::string &message, int line, const char *file)
{
    (void)line;
    (void)file;
    (void)message;
}

} // namespace test
