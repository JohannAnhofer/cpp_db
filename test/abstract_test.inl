template<typename TL, typename TR>
bool test::abstract_test::compare(TL &&left, TR &&right)
{
    return left == right;
}

template<typename Callable, typename ...Args>
void test::abstract_test::test_condition(int line, const char *file, const std::string &name, Callable && callable, Args && ...args)
{
    try
    {
        if (callable(std::forward(args)...))
        {
            statistics.success_count++;
            output->output_success(name, line, file);
        }
        else
        {
            if (expected_message.empty())
            {
                statistics.fail_count++;
                output->output_failure(name, line, file);
            }
            else
            {
                statistics.success_count++;
                output->output_expected_fail(expected_message, name, line, file);
            }
        }
    }
    catch (const std::exception &ex)
    {
        if (expected_message.empty())
        {
            statistics.exception_count++;
            statistics.fail_count++;
            output->output_exception(name, ex.what(), line, file);
        }
        else
        {
            statistics.success_count++;
            output->output_expected_fail(expected_message + "\n   " + ex.what(), name, line, file);
        }
    }
    catch(...)
    {
        if (expected_message.empty())
        {
            statistics.exception_count++;
            statistics.fail_count++;
            output->output_exception(name, "Unknown exception", line, file);
        }
        else
        {
            statistics.success_count++;
            output->output_expected_fail(expected_message + "\n   Unknown exception", name, line, file);
        }
    }

    expected_message.clear();
}

template<typename Exception, typename Callable, typename... Args>
void test::abstract_test::test_for_exception(int line, const char *file, const std::string &name, const std::string &exceptionname, Callable &&code, Args &&... args)
{
	try
	{
		code(std::forward(args)...);

        if (expected_message.empty())
        {
            statistics.fail_count++;
            output->output_failure(name + "\n   Expected exception of type '" + exceptionname + "' not occured!", line, file);
        }
        else
        {
            statistics.success_count++;
            output->output_expected_fail(expected_message, name, line, file);
        }
    }
	catch (const Exception &)
	{
        statistics.success_count++;
        statistics.exception_count++;
        statistics.expected_exception_count++;
        output->output_success(name, line, file);   // expected exception occured
    }
    catch(...)
    {
        statistics.exception_count++;
        statistics.fail_count++;
        output->output_exception(exceptionname, "Unknown exception", line, file);
    }

    expected_message.clear();
}

template<typename Callable, typename... Args>
void test::abstract_test::test_for_no_exception(int line, const char *file, const std::string &name, Callable &&code, Args &&... args)
{
	try
	{
		code(std::forward(args)...);
        statistics.success_count++;
        output->output_success(name, line, file);
	}
	catch (const std::exception &ex)
	{
        if (expected_message.empty())
        {
            statistics.fail_count++;
            statistics.exception_count++;
            output->output_exception(name, ex.what(), line, file);
        }
        else
        {
            statistics.success_count++;
            output->output_expected_fail(expected_message + "\n   " + ex.what(), name, line, file);
        }
    }
    catch(...)
    {
        statistics.exception_count++;
        statistics.fail_count++;
        output->output_exception(name, "Unknown exception", line, file);
    }

    expected_message.clear();
}

