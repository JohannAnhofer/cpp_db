#ifndef TEST_JUNIT_OUTPUT_H
#define TEST_JUNIT_OUTPUT_H

#include "test_output.h"

#include <iosfwd>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <memory>

namespace test
{

class junit_output : public test_output
{
public:
    explicit junit_output(std::ostream *output_stream);
	~junit_output();

    void start() override;
    void start_class(const std::string &class_name) override;
    void start_function(const std::string &class_name, const std::string &function_name) override;
    void end_function(const std::string &class_name, const std::string &function_name) override;
    void end_class(const std::string &class_name, const test_class_statistics &stats) override;
    void end(int class_count, const test_class_statistics &stats) override;

    void output_success(const std::string &message, int line, const char * file) override;
    void output_failure(const std::string &message, int line, const char * file) override;
    void output_exception(const std::string &message, const std::string &exception_message, int line, const char * file) override;
    void output_message(const std::string &message, int line, const char * file) override;
    void output_expected_fail(const std::string &expected, const std::string &message, int line = 0, const char *file = nullptr);

private:
	struct impl;
	std::unique_ptr<impl> pimpl;
};

} // namespace test

#endif // TEST_JUNIT_OUTPUT_H
