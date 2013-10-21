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
    , tiny_output_mode(false)
{
}

abstract_test::~abstract_test()
{
}

void abstract_test::operator()(const std::unordered_set<std::string> &filter)
{
    run(filter);
}

void abstract_test::operator()()
{
    run(std::unordered_set<std::string>());
}

void abstract_test::set_test_stream(std::ostream *os)
{
	if (os)
        output = os;
}

void abstract_test::set_tiny_mode(bool on)
{
    tiny_output_mode = on;
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

void abstract_test::run(const std::unordered_set<std::string> &filter)
{
    init_class_internal();
    for (auto name_and_function : functions)
    {
        if (filter.empty() || (filter.find(name_and_function.first) != std::end(filter)))
        {
            function_count++;
            init_internal(name_and_function.first);
            try
            {
                name_and_function.second();
            }
            catch (const std::exception &ex)
            {
                if (tiny_output_mode)
                    *output << "E";
                else
                    *output << "Exception '" << ex.what() << "' occured" << std::endl;
                fail_count++;
                exception_count++;
            }
            catch (...)
            {
                if (tiny_output_mode)
                    *output << "U";
                else
                    *output << "Unknow exception occured" << std::endl;
                fail_count++;
                exception_count++;
            }
            cleanup_internal(name_and_function.first);
        }
    }
    cleanup_class_internal();
}

void abstract_test::test_message(const std::string &msg)
{
    if (!tiny_output_mode)
        *output << msg << std::endl;
}

void abstract_test::test_condition(const std::string &name, bool cond)
{
	(cond ? success_count : fail_count)++;
    if (tiny_output_mode)
        *output << (cond ? '.' : 'F');
    else
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
    if (tiny_output_mode)
        *output << std::endl;
    else
        *output << function_count << " test functions called" << std::endl
                << success_count + fail_count << " test cases executed" << std::endl
                << fail_count << " test cases failed" << std::endl
                << success_count << " test cases succeded" << std::endl
                << exception_count << " exceptions occured and "
                << expected_exception_count << " where expected" << std::endl;

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

void abstract_test::test_equal(const char * tl, const char * tr, const std::string &name)
{
	test_condition(name, std::string(tl) == std::string(tr));
}

void abstract_test::test_equal(const wchar_t * tl, const wchar_t * tr, const std::string &name)
{
	test_condition(name, std::wstring(tl) == std::wstring(tr));
}

void abstract_test::test_not_equal(const char * tl, const char * tr, const std::string &name)
{
	test_condition(name, std::string(tl) != std::string(tr));
}

void abstract_test::test_not_equal(const wchar_t * tl, const wchar_t * tr, const std::string &name)
{
	test_condition(name, std::wstring(tl) != std::wstring(tr));
}

}
