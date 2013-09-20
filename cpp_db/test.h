#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <functional>
#include <exception>

static const char namefor_ok [] = "ok ";
static const char namefor_nok [] = "NOK";
static const char namefor_exception [] = "EXCEPTION";

std::ostream & test_stream(std::ostream *os = nullptr)
{
	static std::ostream *output = &std::cout;
	
	if (os)
		output = os;

	return *output;
}

void test_message(const std::string &msg)
{
	test_stream() << msg << std::endl;
}

void test_condition(const std::string &name, bool cond)
{
	test_stream() << (cond ? namefor_ok : namefor_nok) << " : " << name << std::endl;
}

void test_condition(const std::string &name, std::function<bool()> cond)
{
	try
	{
		test_stream() << (cond() ? namefor_ok : namefor_nok) << " : " << name << std::endl;
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

#define TEST_EQUAL(left, right) test_equal(left, right, #left##"=="###right)
#define TEST_NOT_EQUAL(left, right) test_not_equal(left, right, #left##"!="###right)
#define TEST_VERIFY(cond) test_condition(#cond, cond)

#endif 
