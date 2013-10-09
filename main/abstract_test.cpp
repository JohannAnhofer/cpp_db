#include "abstract_test.h"

#include <utility>

namespace test
{

abstract_test::abstract_test(const std::string &class_name) 
	: function_count(0)
	, fail_count(0)
	, success_count(0)
	, exception_count(0)
	, expected_exception_count(0)
	, output(&std::cout)
	, test_class_name(class_name)
	, separator(80, '=')
	, namefor_ok("ok")
	, namefor_nok("NOK")
	, namefor_exception("EXCEPTION")
{
}

abstract_test::~abstract_test()
{
}

void abstract_test::operator()()
{
	run();
}

void abstract_test::set_test_stream(std::ostream *os)
{
	if (os)
		output = os;
}

void abstract_test::init_class() 
{
}

void abstract_test::cleanup_class() 
{
}

void abstract_test::init() 
{
}

void abstract_test::cleanup() 
{
}

void abstract_test::add_test_function(std::function<void()> fkt, const std::string &name)
{
	functions.push_back(std::make_pair(name, fkt));
}

void abstract_test::run()
{
	init_class_internal();
	for (auto fn : functions)
	{
		function_count++;
		try
		{
			init_internal(fn.first);
			fn.second();
			cleanup_internal(fn.first);
		}
		catch (...)
		{
			exception_count++;
		}
	}
	cleanup_class_internal();
}

void abstract_test::test_message(const std::string &msg)
{
	*output << msg << std::endl;
}

void abstract_test::test_condition(const std::string &name, bool cond)
{
	(cond ? success_count : fail_count)++;
	*output << (cond ? namefor_ok : namefor_nok) << " : " << name << std::endl;
}

void abstract_test::init_class_internal()
{
	function_count =
		fail_count =
		success_count =
		exception_count =
		expected_exception_count = 0;
	test_message(separator);
	test_message("Starting " + test_class_name + "...");
	test_message(separator);

	TEST_FOR_NO_EXCPTION(init_class());
}

void abstract_test::cleanup_class_internal()
{
	TEST_FOR_NO_EXCPTION(cleanup_class());

	test_message(separator);
	test_message("Finished " + test_class_name);
	test_message(separator);
}

void abstract_test::init_internal(const std::string &fkt_name)
{
	test_message(separator);
	test_message("Run function: " + fkt_name);
	test_message(separator);
	TEST_FOR_NO_EXCPTION(init());
}

void abstract_test::cleanup_internal(const std::string &fkt_name)
{
	TEST_FOR_NO_EXCPTION(cleanup());
	test_message(separator);
	test_message("End function: " + fkt_name);
	test_message(separator);
}

}
