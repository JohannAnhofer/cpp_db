#ifndef TEST_TINY_OUTPUT_H
#define TEST_TINY_OUTPUT_H

#include "test_output.h"

#include <iosfwd>

namespace test
{

class tiny_output : public test_output
{
public:
    explicit tiny_output(std::ostream *output_stream);

    void start() override;
    void start_class(const std::string &class_name) override;
    void start_function(const std::string &class_name, const std::string &function_name) override;
    void end_function(const std::string &class_name, const std::string &function_name) override;
    void end_class(const std::string &class_name, const test_class_statistics &stats) override;
    void end(int class_count, const test_class_statistics &stats) override;

    void output_success(const std::string &message, int line, const char *file) override;
    void output_failure(const std::string &message, int line, const char *file) override;
    void output_exception(const std::string &message, int line, const char *file) override;
    void output_message(const std::string &message, int line, const char *file) override;

private:
    std::ostream *output;
};

} // namespace test

#endif // TEST_TINY_OUTPUT_H