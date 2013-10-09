template<typename Callable, typename ...Args>
void test::abstract_test::test_condition(const std::string &name, Callable && callable, Args && ...args)
{
	try
	{
		bool result = callable(std::forward(args)...);
		(result ? success_count : fail_count)++;
		*output << (result ? namefor_ok : namefor_nok) << " : " << name << std::endl;
	}
	catch (const std::exception &ex)
	{
		exception_count++;
		*output << namefor_exception << " : " << name << " : '" << ex.what() << "'" << std::endl;
	}
}

template<typename TL, typename TR>
void test::abstract_test::test_equal(TL &&tl, TR &&tr, const std::string &name)
{
	try
	{
		test_condition(name, tl == tr);
	}
	catch (const std::exception &ex)
	{
		exception_count++;
		*output << namefor_exception << " : " << name << " : '" << ex.what() << "'" << std::endl;
	}
}

template<typename TL, typename TR>
void test::abstract_test::test_not_equal(TL &&tl, TR &&tr, const std::string &name)
{
	try
	{
		test_condition(name, tl != tr);
	}
	catch (const std::exception &ex)
	{
		exception_count++;
		*output << namefor_exception << " : " << name << " : '" << ex.what() << "'" << std::endl;
	}
}

template<typename Exception, typename Callable, typename... Args>
void test::abstract_test::test_for_exception(const std::string &exceptionname, Callable &&code, Args &&... args)
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
void test::abstract_test::test_for_no_exception(const std::string &message, Callable &&code, Args &&... args)
{
	try
	{
		code(std::forward(args)...);
		test_condition(message, true);
	}
	catch (const std::exception &ex)
	{
		exception_count++;
		std::stringstream str;
		str << namefor_exception << " : " << message << " : '" << ex.what() << "'";
		test_condition(str.str(), false);
	}
}
