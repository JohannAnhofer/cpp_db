#include "windebug_output.h"
#include "normal_output.h"
#include "tiny_output.h"

#if defined(_WIN32) || defined(_WIN64)
#include "windows.h"
#endif

static inline void output_string(const std::string &message)
{
#if defined(_WIN32) || defined(_WIN64)
    OutputDebugStringA(message.c_str());
#else
    (void)message;
#endif
}

namespace test
{

windebug_output::windebug_output(bool striplf, bool use_tiny_mode)
    : generator(use_tiny_mode ? static_cast<test_output*>(new tiny_output(&buffer)) : static_cast<test_output*>(new normal_output(&buffer)))
	, strip_lf(striplf)
{
}

void windebug_output::dump_buffer()
{
	std::string message(buffer.str());
    buffer.str(std::string());
    buffer.clear();

    if (strip_lf)
        dump_string(message);
    else
        output_string(message);
}

void windebug_output::dump_string(const std::string &message) const
{
    if (!message.empty() && (message != "\n"))
    {
        int pos = static_cast<int>(message.find('\n'));
        int lastpos{ 0 };

        if (pos == -1)
            output_string(message);
        else
        {
            while (pos >= 0 && pos < static_cast<int>(message.length()))
            {
                output_string(message.substr(lastpos, pos - lastpos));
                lastpos = pos + 1;
                pos = static_cast<int>(message.find('\n', lastpos));
            }
            if (lastpos < static_cast<int>(message.length()))
                output_string(message.substr(lastpos));
        }
    }
}

void windebug_output::start()
{
	generator->start();
	dump_buffer();
}

void windebug_output::start_class(const std::string &class_name)
{
	generator->start_class(class_name);
	dump_buffer();
}

void windebug_output::start_function(const std::string &class_name, const std::string &function_name)
{
	generator->start_function(class_name, function_name);
	dump_buffer();
}

void windebug_output::end_function(const std::string &class_name, const std::string &function_name)
{
	generator->end_function(class_name, function_name);
	dump_buffer();
}

void windebug_output::end_class(const std::string &class_name, const test_class_statistics &stats)
{
	generator->end_class(class_name, stats);
	dump_buffer();
}

void windebug_output::end(int class_count, const test_class_statistics &stats)
{
	generator->end(class_count, stats);
	dump_buffer();
}

void windebug_output::output_success(const std::string &message, int line, const char *file)
{
	generator->output_success(message, line, file);
	dump_buffer();
}

void windebug_output::output_failure(const std::string &message, int line, const char *file)
{
	generator->output_failure(message, line, file);
	dump_buffer();
}

void windebug_output::output_exception(const std::string &exception_text, const std::string &message, int line, const char *file)
{
	generator->output_exception(exception_text, message, line, file);
	dump_buffer();
}

void windebug_output::output_message(const std::string &message, int line, const char *file)
{
	generator->output_message(message, line, file);
    dump_buffer();
}

void windebug_output::output_expected_fail(const std::string &expected, const std::string &message, int line, const char *file)
{
    generator->output_expected_fail(expected, message, line, file);
    dump_buffer();
}

}
