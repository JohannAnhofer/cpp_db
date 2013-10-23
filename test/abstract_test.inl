template<typename Callable, typename ...Args>
void test::abstract_test::test_condition(const std::string &name, Callable && callable, Args && ...args)
{
	try
	{
		bool result = callable(std::forward(args)...);
        (result ? statistics.success_count : statistics.fail_count)++;
        if (result)
            output->output_success(name);
        else
            output->output_failure(name);
	}
	catch (const std::exception &ex)
	{
        statistics.exception_count++;
        statistics.fail_count++;
        output->output_exception(ex.what());
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
        statistics.exception_count++;
        statistics.fail_count++;
        output->output_exception(ex.what());
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
        statistics.exception_count++;
        statistics.fail_count++;
        output->output_exception(ex.what());
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
        statistics.exception_count++;
        statistics.expected_exception_count++;
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
        output->output_exception(ex.what());
        statistics.fail_count++;
        statistics.exception_count++;
    }
}
