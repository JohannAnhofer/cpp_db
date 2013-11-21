#include "junit_output.h"
#include "test_class_statistics.h"

#include <stdexcept>
#include <ostream>
#include <ctime>
#include <vector>

namespace test
{

using timepoint = std::chrono::time_point<std::chrono::system_clock>;
static const char ISO8601_DATE_TIME_FORMAT[] = "%Y-%m-%dT%H:%M:%S";

// Get current date/time, format is ISO8601
static std::string get_current_timestamp_iso8601()
{
	time_t now = time(0);
#ifdef _MSC_VER
	tm tm_now;
	localtime_s(&tm_now, &now);
#else
	const tm  tm_now = *localtime(&now);
#endif 
	char buf[80];

	if (strftime(buf, sizeof(buf), ISO8601_DATE_TIME_FORMAT, &tm_now))
		return buf;
	else
		return std::string();
}

static inline double duration_in_seconds(const timepoint &start, const timepoint &end) 
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
}

std::string encode_for_xml(const std::string &raw_input)
{
	std::string result;

	for (auto c : raw_input)
	{
		switch (c)
		{
		case '<':
			result.append("&lt;");
			break;
		case '>':
			result.append("&gt;");
			break;
		case '&':
			result.append("&amp;");
			break;
		case '"':
			result.append("&quot;");
			break;
		case '\'':
			result.append("&apos;");
			break;
		default:
			if (c < 32 || c >= 127)
				result.append("&#" + std::to_string(static_cast<unsigned char>(c)) + ";");
			else
				result.append(&c, 1);
			break;
		}
	}
	return result;
}

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

struct junit_output::impl
{
	explicit impl(std::ostream *output_stream);

	// functions
	void add_test_case(const std::string &type, const std::string &message, int line, const char * file);
	void write_xml() const;

	// data
	std::ostream *output;
	std::map<std::string, test_suite> test_suites;
	test_function *current_test_function;
};

junit_output::impl::impl(std::ostream *output_stream)
: output{ output_stream }
, current_test_function{ nullptr }
{
}

void junit_output::impl::write_xml() const
{
	*output << "<?xml version = '1.0'"
		<< " encoding = '" << "ASCII" << "'?>\n"
		<< "<testsuites"
		<< " xmlns:xsi='" << "http://www.w3.org/2001/XMLSchema-instance" << "'"
		<< " xsi:noNamespaceSchemaLocation='" << "http://windyroad.org/dl/Open%20Source/JUnit.xsd" << "'"
		<< ">\n";

	int id{ 0 };

	for (const auto &suite_entry : test_suites)
	{
		const auto &suite = suite_entry.second;

		*output << "\t<testsuite"
			<< " id='" << id++ << "'"
			<< " package='" << suite_entry.first << "'"
			<< " name='" << suite.name << "'"
			<< " timestamp='" << suite.timestamp << "'"
			<< " hostname='" << suite.hostname << "'"
			<< " tests='" << suite.tests << "'"
			<< " failures='" << suite.failures << "'"
			<< " errors='" << suite.errors << "'"
			<< " time='" << duration_in_seconds(suite.start, suite.end) << "'"
			<< ">\n"
			<< "\t\t<properties />\n";

		int tc_count{ 0 };

		for (const auto &tf : suite.test_functions)
		{
			const auto &test_function = tf.second;

			for (const auto &test_case : test_function.test_cases)
			{
				*output << "\t\t<testcase"
					<< " name='" << "test_case_" << tc_count++ << "_line_" << test_case.line << "'"
					<< " classname='" << test_function.class_name << "::" << test_function.name << "'"
					<< " time='" << duration_in_seconds(test_case.start, test_case.end) << "'";

				if (test_case.failure_type.empty())
					*output << " />\n";
				else
					*output << " >\n"
					<< "\t\t\t<" << test_case.failure_type
					<< " type='" << test_case.failure_type << "'"
					<< " message='" << encode_for_xml(test_case.message) << "'"
					<< " />\n"
					<< "\t\t</testcase>\n";
			}
		}

		*output << "\t\t<system-out />\n"
			<< "\t\t<system-err />\n"
			<< "\t</testsuite>\n";
	}
	*output << "</testsuites>\n";
}

void junit_output::impl::add_test_case(const std::string &type, const std::string &message, int line, const char * file)
{
	if (current_test_function)
	{
		test_case tcase;

		tcase.message = message;
		tcase.failure_type = type;
		if (current_test_function->test_cases.empty())
			tcase.start = current_test_function->start;
		else
			tcase.start = current_test_function->test_cases.back().end;
		tcase.end = std::chrono::system_clock::now();
		tcase.line = line;
		tcase.file = file;
		current_test_function->test_cases.push_back(tcase);
	}
}

// junit_output implemenation 

junit_output::junit_output(std::ostream *output_stream)
	: pimpl(new impl(output_stream))
{
    if (!pimpl->output)
        throw std::runtime_error("No outputstream");
}

junit_output::~junit_output()
{
}

void junit_output::start()
{
	pimpl->test_suites.clear();
}

void junit_output::start_class(const std::string &class_name)
{
	test_suite suite;
	suite.name = class_name;
	suite.timestamp = get_current_timestamp_iso8601();
	suite.hostname = "localhost";
	suite.tests = suite.failures = suite.errors = 0;
	suite.end = suite.start = std::chrono::system_clock::now();
	pimpl->test_suites.insert(std::make_pair(class_name, suite));
}

void junit_output::start_function(const std::string &class_name, const std::string &function_name)
{
	test_function function;
	function.name = function_name;
	function.class_name = class_name;
	function.end = function.start = std::chrono::system_clock::now();
	pimpl->test_suites[class_name].test_functions.insert(make_pair(function_name, function));
	pimpl->current_test_function = &pimpl->test_suites[class_name].test_functions[function_name];
}

void junit_output::end_function(const std::string &class_name, const std::string &function_name)
{
	pimpl->test_suites[class_name].test_functions[function_name].end = std::chrono::system_clock::now();
	pimpl->current_test_function = nullptr;
}

void junit_output::end_class(const std::string &class_name, const test_class_statistics &stats)
{
	auto &test_suite = pimpl->test_suites[class_name];

	test_suite.end = std::chrono::system_clock::now();
	test_suite.tests = stats.success_count + stats.fail_count;
	test_suite.failures = stats.fail_count - (stats.exception_count - stats.expected_exception_count);
	test_suite.errors = stats.exception_count - stats.expected_exception_count;
}

void junit_output::end(int /*class_count*/, const test_class_statistics &/*stats*/)
{
	pimpl->write_xml();
	pimpl->test_suites.clear();
	pimpl->current_test_function = nullptr;
}

void junit_output::output_success(const std::string &message, int line, const char *file)
{
	pimpl->add_test_case(std::string(), message, line, file);
}

void junit_output::output_failure(const std::string &message, int line, const char *file)
{
	pimpl->add_test_case("failure", message, line, file);
}

void junit_output::output_exception(const std::string &message, const std::string &exception_message, int line, const char *file)
{
    pimpl->add_test_case("error", exception_message + " : " + message, line, file);
}

void junit_output::output_message(const std::string &/*message*/, int /*line*/, const char * /*file*/)
{
}

void junit_output::output_expected_fail(const std::string &expected, const std::string &message, int line, const char *file)
{
	pimpl->add_test_case(std::string(), expected + " : " + message, line, file);
}

} // namespace test
