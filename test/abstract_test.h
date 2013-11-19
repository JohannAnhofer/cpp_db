#ifndef TEST_ABSTRACT_TEST_H
#define TEST_ABSTRACT_TEST_H

#include "test_output.h"
#include "test_class_statistics.h"

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

    void operator()(const std::unordered_set<std::string> &filter);

    void set_output(const std::shared_ptr<test_output> &poutput);

    test_class_statistics get_statistics() const;

protected:
	virtual void init_class();
	virtual void cleanup_class();
	virtual void init();
	virtual void cleanup();

    void next_test_is_expected_to_fail(const std::string &message);

    using test_function = std::function<void()>;
	void add_test_function(test_function fkt, const std::string &name);

    template<typename Callable, typename ...Args> void test_condition(int line, const char *file, const std::string &name, Callable && callable, Args && ...args);
    template<typename Exception, typename Callable, typename... Args> void test_for_exception(int line, const char *file, const std::string &name, const std::string &exceptionname, Callable &&code, Args &&... args);
    template<typename Callable, typename... Args> void test_for_no_exception(int line, const char *file, const std::string &name, Callable &&code, Args &&... args);

    template<typename TL, typename TR> bool compare(TL &&left, TR &&right);
    bool compare(const char *left, const char *right);
    bool compare(char * left, char *right);
    bool compare(const char * left, char *right);
    bool compare(char * left, const char *right);
    bool compare(const wchar_t *left, const wchar_t *right);
    bool compare(wchar_t * left, wchar_t * right);
    bool compare(const wchar_t * left, wchar_t * right);
    bool compare(wchar_t * left, const wchar_t * right);

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
    std::string expected_message;
};

}

#include "abstract_test.inl"

#define TEST_EQUAL(left, right) test_condition(__LINE__, __FILE__, #left"=="#right, [&]() -> bool {return compare(left, right);})
#define TEST_NOT_EQUAL(left, right) test_condition(__LINE__, __FILE__, #left"!="#right, [&]() -> bool {return !compare(left, right);})
#define TEST_VERIFY(cond) test_condition(__LINE__, __FILE__, #cond, [&]() -> bool {return cond;})
#define TEST_FOR_EXCEPTION(code, excpt) test_for_exception<excpt>(__LINE__, __FILE__, #code, #excpt, [&](){code;})
#define TEST_FOR_NO_EXCEPTION(code) test_for_no_exception(__LINE__, __FILE__, #code, [&](){code;})

#define BEGIN_DECLARE_TEST_CLASS(class_name) class_name() : abstract_test(#class_name) {
#define DECLARE_TEST_FUNCTION(fkt_name) add_test_function([&](){ fkt_name(); }, #fkt_name);
#define END_DECLARE_TEST_CLASS() }

#endif 
