#include "test_app.h"

#include "tiny_output.h"
#include "normal_output.h"
#include "junit_output.h"

#include <algorithm>
#include <sstream>

namespace test
{

using stringlist = std::vector<std::string>;
using filter_type = std::unordered_set<std::string>;

static const char ARG_COMMAND_PREFIX[] = "--";
static const char ARG_FILTER_CLASSES_LIST[] = "--classes";
static const char ARG_FILTER_FUNCTIONS_LIST[] = "--functions";
static const char ARG_HELP[] = "--help";
static const char ARG_JUNIT_OUTPUT[] = "--junit";
static const char ARG_CERR[] = "--cerr";
static const char ARG_TINY_MODE[] = "--tiny";
static const char ARG_OUTPUT_TO_FILE[] = "--file";

static const char ARG_SHORT_COMMAND_PREFIX[] = "-";
static const char ARG_SHORT_FILTER_CLASSES_LIST[] = "-c";
static const char ARG_SHORT_FILTER_FUNCTIONS_LIST[] = "-f";
static const char ARG_SHORT_HELP[] = "-h";
static const char ARG_SHORT_JUNIT_OUTPUT[] = "-j";
static const char ARG_SHORT_CERR[] = "-r";
static const char ARG_SHORT_TINY_MODE[] = "-t";
static const char ARG_SHORT_OUTPUT_TO_FILE[] = "-f";

static stringlist::const_iterator locate_command(const stringlist &args, const std::string &command, const std::string &short_command);
static bool contains(const stringlist &args, const std::string &command, const std::string &short_command);
static void show_usage();
static filter_type extract_filter(const stringlist &args, const std::string &filter_command, const std::string &short_filter_command);

test_app::test_app(int argc, char *argv[])
	: help_requested(false)
{
    stringlist args(argv + 1, argv + argc);

    help_requested = contains(args, ARG_HELP, ARG_SHORT_HELP);

	filter_classes = extract_filter(args, ARG_FILTER_CLASSES_LIST, ARG_SHORT_FILTER_CLASSES_LIST);
	filter_functions = extract_filter(args, ARG_FILTER_FUNCTIONS_LIST, ARG_SHORT_FILTER_FUNCTIONS_LIST);
    filter_type files = extract_filter(args, ARG_OUTPUT_TO_FILE, ARG_SHORT_OUTPUT_TO_FILE);

    if (files.size() > 1)
        throw std::runtime_error("Too much files for output!");

    bool junit_requested = contains(args, ARG_JUNIT_OUTPUT, ARG_SHORT_JUNIT_OUTPUT);
    bool cerr = contains(args, ARG_CERR, ARG_SHORT_CERR);
    bool use_tiny_mode = contains(args, ARG_TINY_MODE, ARG_SHORT_TINY_MODE);
    std::ostream *output_stream = cerr ? &std::cerr : &std::cout;

    if (!files.empty())
    {
        output_file.open(*files.begin(), std::ofstream::out | std::ofstream::trunc);
        output_stream = &output_file;
    }

    if (junit_requested)
        output = std::make_shared<junit_output>(output_stream);
    else if (use_tiny_mode)
        output = std::make_shared<tiny_output>(output_stream);
    else
        output = std::make_shared<normal_output>(output_stream);
}

void test_app::run()
{
	if (help_requested)
		show_usage();
	else
    {
        output->start();

        int tc_count(0);
        test_class_statistics stats;

		std::for_each(std::begin(classes), std::end(classes), 
			[&](const test_class &tc)
			{
				if (filter_classes.empty() || (filter_classes.find(tc.first) != std::end(filter_classes)))
                {
                    stats += tc.second();
                    tc_count++;
                }
            }
		);
        output->end(tc_count, stats);
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
    std::cout << "-j, --junit       ... Output is a junit compatible XML.\n";
    std::cout << "-c, --classes     ... provide a space separated list of class names to apply as filter\n";
    std::cout << "-f, --functions   ... provide a space separated list of function names to apply as filter\n";
    std::cout << "-r, --cerr        ... send output to standard error\n";
    std::cout << "-t, --tiny        ... show only one character per test case\n";
    std::cout << "-f, --file <name> ... send output to file\n";
	std::cout << std::endl;
}

filter_type extract_filter(const stringlist &args, const std::string &filter_command, const std::string &short_filter_command)
{
    auto start_filter = locate_command(args, filter_command, short_filter_command);
    if (start_filter != std::end(args))
    {
        auto start_filter_args = start_filter + 1;
        auto end_filter = std::find_if(start_filter_args, std::end(args), [](const std::string &arg){return arg.substr(0,1) == ARG_SHORT_COMMAND_PREFIX;});
        return filter_type(start_filter_args, end_filter);
    }
    return filter_type();
}

}

