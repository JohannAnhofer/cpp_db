#include "test_app.h"

#include <algorithm>

namespace test
{

using stringlist = std::vector<std::string>;
using filter_type = std::unordered_set<std::string>;

static const char ARG_COMMAND_PREFIX[] = "--";
static const char ARG_FILTER_CLASSES_LIST[] = "--classes";
static const char ARG_FILTER_FUNCTIONS_LIST[] = "--functions";
static const char ARG_HELP[] = "--help";
static const char ARG_JUNIT_OUTPUT[] = "--junit";

static const char ARG_SHORT_COMMAND_PREFIX[] = "-";
static const char ARG_SHORT_FILTER_CLASSES_LIST[] = "-c";
static const char ARG_SHORT_FILTER_FUNCTIONS_LIST[] = "-f";
static const char ARG_SHORT_HELP[] = "-h";
static const char ARG_SHORT_JUNIT_OUTPUT[] = "-j";

static stringlist::const_iterator locate_command(const stringlist &args, const std::string &command, const std::string &short_command);
static bool contains(const stringlist &args, const std::string &command, const std::string &short_command);
static void show_usage();
static filter_type extract_filter(stringlist &args, const std::string &filter_command, const std::string &short_filter_command);

test_app::test_app(int argc, char *argv[])
	: help_requested(false)
	, junit_requested(false)
{
    stringlist args(argv + 1, argv + argc);

	filter_classes = extract_filter(args, ARG_FILTER_CLASSES_LIST, ARG_SHORT_FILTER_CLASSES_LIST);
	filter_functions = extract_filter(args, ARG_FILTER_FUNCTIONS_LIST, ARG_SHORT_FILTER_FUNCTIONS_LIST);
	help_requested = contains(args, ARG_HELP, ARG_SHORT_HELP);
	junit_requested = contains(args, ARG_JUNIT_OUTPUT, ARG_SHORT_JUNIT_OUTPUT);
}

void test_app::run()
{
	if (help_requested)
		show_usage();
	else
	{
		std::for_each(std::begin(classes), std::end(classes), 
			[&](const test_class &tc)
			{
				if (filter_classes.empty() || (filter_classes.find(tc.first) != std::end(filter_classes)))
					tc.second(filter_functions);
			}
		);
	}
}

stringlist::const_iterator locate_command(const stringlist &args, const std::string &command, const std::string &short_command)
{
	return std::find_if(std::begin(args), std::end(args), [&command, &short_command](const std::string &i){return i == command || i == short_command; });
}

bool contains(const stringlist &args, const std::string &command, const std::string &short_command)
{
	return locate_command(args, command, short_command) != std::end(args);
}

void show_usage()
{
	std::cout << "\nUnittest application.\n";
	std::cout << "by DI Johann Anhofer in 2013\n";
	std::cout << "Usage: test_app.exe [--junit] [--classes <class> [<class>...] [--functions <function> [<function>...]]]|[--help]\n";
	std::cout << "--junit     ... Output is a junit compatible XML.\n";
	std::cout << "--classes   ... provide a space separated list of class names to apply as filter\n";
	std::cout << "--functions ... provide a space separated list of function names to apply as filter\n";
	std::cout << std::endl;
}

filter_type extract_filter(stringlist &args, const std::string &filter_command, const std::string &short_filter_command)
{
	auto start_filter = locate_command(args, filter_command, short_filter_command);
    if (start_filter != std::end(args))
    {
        auto start_filter_args = start_filter + 1;
        auto end_filter = std::find_if(start_filter_args, std::cend(args), [](const std::string &arg){return arg.substr(0,1) == ARG_SHORT_COMMAND_PREFIX;});
		filter_type result(start_filter_args, end_filter);
        args.erase(start_filter, end_filter);
		return result;
    }
    return filter_type();
}

}

