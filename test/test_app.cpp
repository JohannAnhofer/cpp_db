#include "test_app.h"

#include <algorithm>

namespace test
{

static const char ARG_COMMAND_PREFIX[] = "--";
static const char ARG_FILTER_CLASSES_LIST[] = "--classes";
static const char ARG_FILTER_FUNCTIONS_LIST[] = "--functions";
static const char ARG_HELP[] = "--help";

test_app::test_app(int argc, char *argv[])
{
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i)
        args.push_back(argv[i]);

    filter_classes = extract_filter(args, ARG_FILTER_CLASSES_LIST);
    filter_functions = extract_filter(args, ARG_FILTER_FUNCTIONS_LIST);
}

void test_app::run()
{
	for (auto test_class : classes)
	{
		if (filter_classes.empty() || (filter_classes.find(test_class.first) != std::end(filter_classes)))
			test_class.second(filter_functions);
	}
}

test_app::test_classes::const_iterator test_app::find_test_class(const std::string &tc_name) const
{
    return std::find_if(std::begin(classes), std::end(classes), [&](const test_class &tc) {return tc.first == tc_name;});
}

test_app::filter_type test_app::extract_filter(std::vector<std::string> &args, const std::string &filter_command)
{
	filter_type result;
    auto start_filter = std::find(std::begin(args), std::end(args), filter_command);
    if (start_filter != std::end(args))
    {
        auto start_filter_args = start_filter + 1;
        auto end_filter = std::find_if(start_filter_args, std::end(args), [](const std::string &arg){return arg.substr(0,2) == ARG_COMMAND_PREFIX;});
        for (auto pos = start_filter_args; pos != end_filter; ++pos)
            result.insert(*pos);
        args.erase(start_filter, end_filter);
    }
    return result;
}

}

