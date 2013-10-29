#ifndef TEST_JUNIT_OUTPUT_H
#define TEST_JUNIT_OUTPUT_H

#include "test_output.h"

#include <iosfwd>
#include <string>
#include <vector>
#include <map>
#include <chrono>

namespace test
{

class junit_output : public test_output
{
public:
    explicit junit_output(std::ostream *output_stream);

    void start() override;
    void start_class(const std::string &class_name) override;
    void start_function(const std::string &class_name, const std::string &function_name) override;
    void end_function(const std::string &class_name, const std::string &function_name) override;
    void end_class(const std::string &class_name, const test_class_statistics &stats) override;
    void end(int class_count, const test_class_statistics &stats) override;

    void output_success(const std::string &message, int line, const char * file) override;
    void output_failure(const std::string &message, int line, const char * file) override;
    void output_exception(const std::string &message, int line, const char * file) override;
    void output_message(const std::string &message, int line, const char * file) override;


private:
	// types
	using timepoint = std::chrono::time_point<std::chrono::system_clock>;

	struct test_case
	{
		std::string message;
		std::string failure_type;
		timepoint start;
		timepoint end;
		int line;
		const char *file;
	};

	struct test_function
	{
		std::string name;
		std::string class_name;
		timepoint start;
		timepoint end;
		std::vector<test_case> test_cases;
	};
	
	struct test_suite
	{
		std::string name;
		std::string timestamp;
		std::string hostname;
		int tests, failures, errors;
		timepoint start;
		timepoint end;
		std::map<std::string, test_function> test_functions;
	};

	// functions
	void add_test_case(const std::string &type, const std::string &message, int line, const char * file);
	void write_xml() const;

	// data
	std::ostream *output;
	std::map<std::string, test_suite> test_suites;
	test_function *current_test_function;	
};

} // namespace test

#endif // TEST_JUNIT_OUTPUT_H
