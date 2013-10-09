#ifndef TEST_ABSTRACT_TEST_H
#define TEST_ABSTRACT_TEST_H

#include <iostream>
#include <string>
#include <functional>
#include <exception>
#include <sstream>
#include <vector>

namespace test
{

class abstract_test
{
public:
	abstract_test(const std::string &class_name);
	virtual ~abstract_test() = 0;

	void set_test_stream(std::ostream *os);
	void operator()();

protected:
	virtual void init_class();
	virtual void cleanup_class();
	virtual void init();
	virtual void cleanup();

	using test_function = std::function<void()>;
	void add_test_function(test_function fkt, const std::string &name);

	void test_message(const std::string &msg);
	void test_condition(const std::string &name, bool cond);
	template<typename Callable, typename ...Args> void test_condition(const std::string &name, Callable && callable, Args && ...args);
	template<typename TL, typename TR> void test_equal(TL &&tl, TR &&tr, const std::string &name);
	template<typename TL, typename TR> void test_not_equal(TL &&tl, TR &&tr, const std::string &name);
	template<typename Exception, typename Callable, typename... Args> void test_for_exception(const std::string &exceptionname, Callable &&code, Args &&... args);
	template<typename Callable, typename... Args> void test_for_no_exception(const std::string &message, Callable &&code, Args &&... args);

private:
	void init_class_internal();
	void cleanup_class_internal();
	void init_internal(const std::string &fkt_name);
	void cleanup_internal(const std::string &fkt_name);
	void run();

private:
    int function_count, fail_count, success_count, exception_count, expected_exception_count;
    std::ostream *output;
	using test_function_and_name = std::pair<std::string, test_function>;
	using test_functions = std::vector< test_function_and_name >;
	test_functions functions;
	std::string test_class_name, separator;
	std::string namefor_ok, namefor_nok, namefor_exception;
};

}

#include "abstract_test.inl"

#define TEST_EQUAL(left, right) test_equal(left, right, #left"=="#right)
#define TEST_NOT_EQUAL(left, right) test_not_equal(left, right, #left"!="#right)
#define TEST_VERIFY(cond) test_condition(#cond, cond)
#define TEST_VERIFY_RESULT(code) test_condition(#code, [&](){code;})
#define TEST_FOR_EXCEPTION(code, excpt) test_for_exception<excpt>(#excpt, [&](){code;})
#define TEST_FOR_NO_EXCPTION(code) test_for_no_exception(#code, [&](){code;})

#define BEGIN_DECLARE_TEST_CLASS(class_name) class_name() : abstract_test(#class_name) {
#define DECLARE_TEST_FUNCTION(fkt_name) add_test_function([&](){ fkt_name(); }, #fkt_name);
#define END_DECLARE_TEST_CLASS() }

#endif 
