#ifndef TEST_ABSTRACT_TEST_H
#define TEST_ABSTRACT_TEST_H

#include "test_output.h"

#include <iostream>
#include <string>
#include <functional>
#include <exception>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <memory>

namespace test
{

class abstract_test
{
public:
    explicit abstract_test(const std::string &class_name);
	virtual ~abstract_test() = 0;

	void operator()();
    void operator()(const std::unordered_set<std::string> &filter);

    void set_output(const std::shared_ptr<test_output> &poutput);

    test_class_statistics get_statistics() const;

protected:
	virtual void init_class();
	virtual void cleanup_class();
	virtual void init();
	virtual void cleanup();

	using test_function = std::function<void()>;
	void add_test_function(test_function fkt, const std::string &name);

	void test_condition(const std::string &name, bool cond);
	template<typename Callable, typename ...Args> void test_condition(const std::string &name, Callable && callable, Args && ...args);
	template<typename TL, typename TR> void test_equal(TL &&tl, TR &&tr, const std::string &name);
	template<typename TL, typename TR> void test_not_equal(TL &&tl, TR &&tr, const std::string &name);
	template<typename Exception, typename Callable, typename... Args> void test_for_exception(const std::string &exceptionname, Callable &&code, Args &&... args);
	template<typename Callable, typename... Args> void test_for_no_exception(const std::string &message, Callable &&code, Args &&... args);

	// overloads for character arrays 
	void test_equal(const char * tl, const char * tr, const std::string &name);
	void test_equal(const wchar_t * tl, const wchar_t * tr, const std::string &name);
	void test_not_equal(const char * tl, const char * tr, const std::string &name);
	void test_not_equal(const wchar_t * tl, const wchar_t * tr, const std::string &name);

private:
	void init_class_internal();
	void cleanup_class_internal();
	void init_internal(const std::string &fkt_name);
	void cleanup_internal(const std::string &fkt_name);
    void run(const std::unordered_set<std::string> &filter);

private:
    test_class_statistics statistics;
    std::shared_ptr<test_output> output;
	using test_function_and_name = std::pair<std::string, test_function>;
	using test_functions = std::vector< test_function_and_name >;
	test_functions functions;
    std::string test_class_name;
};

}

#include "abstract_test.inl"

#define TEST_EQUAL(left, right) test_equal(left, right, #left"=="#right)
#define TEST_NOT_EQUAL(left, right) test_not_equal(left, right, #left"!="#right)
#define TEST_VERIFY(cond) test_condition(#cond, cond)
#define TEST_VERIFY_RESULT(code) test_condition(#code, [&]() -> bool {code;})
#define TEST_FOR_EXCEPTION(code, excpt) test_for_exception<excpt>(#excpt, [&](){code;})
#define TEST_FOR_NO_EXCPTION(code) test_for_no_exception(#code, [&](){code;})

#define BEGIN_DECLARE_TEST_CLASS(class_name) class_name() : abstract_test(#class_name) {
#define DECLARE_TEST_FUNCTION(fkt_name) add_test_function([&](){ fkt_name(); }, #fkt_name);
#define END_DECLARE_TEST_CLASS() }

#endif 
