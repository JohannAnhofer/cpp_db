#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <functional>
#include <exception>
#include <sstream>
#include <vector>

static const char namefor_ok [] = "ok ";
static const char namefor_nok [] = "NOK";
static const char namefor_exception [] = "EXCEPTION";

inline std::ostream & test_stream(std::ostream *os = nullptr)
{
	static std::ostream *output = &std::cout;
	
	if (os)
		output = os;

	return *output;
}

inline void test_message(const std::string &msg)
{
	test_stream() << msg << std::endl;
}

inline void test_condition(const std::string &name, bool cond)
{
	test_stream() << (cond ? namefor_ok : namefor_nok) << " : " << name << std::endl;
}

template<typename Callable, typename ...Args>
inline void test_condition(const std::string &name, Callable && callable, Args && ...args)
{
	try
	{
        test_stream() << (callable(std::forward(args)...) ? namefor_ok : namefor_nok) << " : " << name << std::endl;
	}
	catch (const std::exception &ex)
	{
		test_stream() << namefor_exception << " : " << name << " : '" << ex.what() << "'" << std::endl;
	}
}

template<typename TL, typename TR>
void test_equal(TL &&tl, TR &&tr, const std::string &name)
{
	try
	{
		test_condition(name, tl == tr);
	}
	catch (const std::exception &ex)
	{
		test_stream() << namefor_exception << " : " << name << " : '" << ex.what() << "'" << std::endl;
	}
}

template<typename TL, typename TR>
void test_not_equal(TL &&tl, TR &&tr, const std::string &name)
{
	try
	{
		test_condition(name, tl != tr);
	}
	catch (const std::exception &ex)
	{
		test_stream() << namefor_exception << " : " << name << " : '" << ex.what() << "'" << std::endl;
	}
}

template<typename Exception, typename Callable, typename... Args>
void test_for_exception(const std::string &exceptionname, Callable &&code, Args &&... args)
{
	try
	{
        code(std::forward(args)...);
		test_condition("Expected exception of type '" + exceptionname + "' not occured!", false);
	}
	catch (const Exception &ex)
	{
		test_condition(std::string("Expected exception of type '" + exceptionname + "' occured : ") + ex.what(), true);
	}
}

template<typename Callable, typename... Args>
void test_for_no_exception(const std::string &message, Callable &&code, Args &&... args)
{
	try
	{
        code(std::forward(args)...);
		test_condition(message, true);
	}
	catch (const std::exception &ex)
	{
		std::stringstream str;
		str << namefor_exception << " : " << message << " : '" << ex.what() << "'";
		test_condition(str.str(), false);
	}
}

#define TEST_EQUAL(left, right) test_equal(left, right, #left"=="#right)
#define TEST_NOT_EQUAL(left, right) test_not_equal(left, right, #left"!="#right)
#define TEST_VERIFY(cond) test_condition(#cond, cond)
#define TEST_VERIFY_RESULT(code) test_condition(#code, [&](){code;})
#define TEST_FOR_EXCEPTION(code, excpt) test_for_exception<excpt>(#excpt, [&](){code;})
#define TEST_FOR_NO_EXCPTION(code) test_for_no_exception(#code, [&](){code;})


class Test
{
public:
    Test() :
        functionCount(0)
      , failCount(0)
      , successCount(0)
      , exceptionCount(0)
      , output(&std::cout)
    {}

    virtual ~Test() {}
    virtual void init_class() {}
    virtual void cleanup_class() {}
    virtual void init() {}
    virtual void cleanup() {}

    void add_test_function(std::function<void()> fkt)
    {
        functions.push_back(fkt);
    }

    void run()
    {
        init_class();
        for(auto f : functions)
        {
            functionCount++;
            try
            {
                init();
                f();
                cleanup();
            }
            catch(...)
            {
                exceptionCount++;
            }
        }
        cleanup_class();
    }

    void set_test_stream(std::ostream *os)
    {
        if (os)
            output = os;
    }

protected:
    inline void test_message(const std::string &msg)
    {
        *output << msg << std::endl;
    }

private:
    int functionCount, failCount, successCount, exceptionCount;
    std::ostream *output;
    std::vector< std::function<void()> > functions;
};

#endif 
