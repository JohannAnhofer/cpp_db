#include "tiny_output.h"
#include "test_class_statistics.h"

#include <stdexcept>
#include <ostream>

namespace test
{

tiny_output::tiny_output(std::ostream *output_stream)
    : output(output_stream)
{
    if (!output)
        throw std::runtime_error("No outputstream");
}

void tiny_output::start()
{
    *output << "\n";
}

void tiny_output::start_class(const std::string &)
{
}

void tiny_output::start_function(const std::string &, const std::string &)
{
}

void tiny_output::end_function(const std::string &, const std::string &)
{
}

void tiny_output::end_class(const std::string &, const test_class_statistics &)
{
    *output << "\n";
}

void tiny_output::end(int, const test_class_statistics &)
{
}

void tiny_output::output_success(const std::string &, int, const char *)
{
    *output << ".";
}

void tiny_output::output_failure(const std::string &, int, const char *)
{
    *output << "F";
}

void tiny_output::output_exception(const std::string &, const std::string &, int, const char *)
{
    *output << "E";
}

void tiny_output::output_message(const std::string &, int, const char *)
{
}

void tiny_output::output_expected_fail(const std::string &, const std::string &, int, const char *)
{
    *output << "X";
}

} // namespace test
