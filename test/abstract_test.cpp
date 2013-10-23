#include "abstract_test.h"

#include <utility>

namespace test
{

abstract_test::abstract_test(const std::string &class_name)
    : test_class_name(class_name)
{
}

abstract_test::~abstract_test()
{
}

void abstract_test::operator()(const std::unordered_set<std::string> &filter)
{
    run(filter);
}

void abstract_test::set_output(const std::shared_ptr<test_output> &poutput)
{
    output = poutput;
}

test_class_statistics abstract_test::get_statistics() const
{
    return statistics;
}

void abstract_test::operator()()
{
    run(std::unordered_set<std::string>());
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
    if (!output)
        throw std::runtime_error("No output destination defined!");

    init_class_internal();
    for (auto name_and_function : functions)
    {
        if (filter.empty() || (filter.find(name_and_function.first) != std::end(filter)))
        {
            statistics.function_count++;
            init_internal(name_and_function.first);
            try
            {
                name_and_function.second();
            }
            catch (const std::exception &ex)
            {
                output->output_exception(ex.what());
                statistics.fail_count++;
                statistics.exception_count++;
            }
            catch (...)
            {
                output->output_exception("Unknow exception occured");
                statistics.fail_count++;
                statistics.exception_count++;
            }
            cleanup_internal(name_and_function.first);
        }
    }
    cleanup_class_internal();
}

void abstract_test::test_condition(const std::string &name, bool cond)
{
    (cond ? statistics.success_count : statistics.fail_count)++;
    if (cond)
        output->output_success(name);
    else
        output->output_failure(name);
}

void abstract_test::init_class_internal()
{    
    statistics = test_class_statistics();
    output->start_class(test_class_name);

    TEST_FOR_NO_EXCPTION(init_class());
}

void abstract_test::cleanup_class_internal()
{
	TEST_FOR_NO_EXCPTION(cleanup_class());

    output->end_class(test_class_name, statistics);
}

void abstract_test::init_internal(const std::string &fkt_name)
{
    output->start_function(test_class_name, fkt_name);
	TEST_FOR_NO_EXCPTION(init());
}

void abstract_test::cleanup_internal(const std::string &fkt_name)
{
	TEST_FOR_NO_EXCPTION(cleanup());
    output->end_function(test_class_name, fkt_name);
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
