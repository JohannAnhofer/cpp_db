#include "test_app.h"

#include "tiny_output.h"
#include "normal_output.h"
#include "junit_output.h"
#include "windebug_output.h"
#include "test_class_statistics.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace test
{

using stringlist = std::vector<std::string>;
using filter_type = std::unordered_set<std::string>;

static const char arg_command_prefix[] = "--";
static const char arg_filter_classes_list[] = "--classes";
static const char arg_filter_functions_list[] = "--functions";
static const char arg_help[] = "--help";
static const char arg_junit_output[] = "--junit";
static const char arg_cerr[] = "--cerr";
static const char arg_tiny_mode[] = "--tiny";
static const char arg_output_to_file[] = "--file";

static const char arg_short_command_prefix[] = "-";
static const char arg_short_filter_classes_list[] = "-c";
static const char arg_short_filter_functions_list[] = "-f";
static const char arg_short_help[] = "-h";
static const char arg_short_junit_output[] = "-j";
static const char arg_short_cerr[] = "-r";
static const char arg_short_tiny_mode[] = "-t";
static const char arg_short_output_to_file[] = "-f";

static const char arg_windebug_mode[] = "--windebug";
static const char arg_short_windebug_mode[] = "-w";

static stringlist::const_iterator locate_command(const stringlist &args, const std::string &command, const std::string &short_command);
static bool contains(const stringlist &args, const std::string &command, const std::string &short_command);
static void show_usage();
static filter_type extract_filter(const stringlist &args, const std::string &filter_command, const std::string &short_filter_command);

test_app::test_app(int argc, char *argv[])
	: help_requested(false)
{
    stringlist args(argv + 1, argv + argc);

    help_requested = contains(args, arg_help, arg_short_help);

    filter_classes = extract_filter(args, arg_filter_classes_list, arg_short_filter_classes_list);
    filter_functions = extract_filter(args, arg_filter_functions_list, arg_short_filter_functions_list);
    filter_type files = extract_filter(args, arg_output_to_file, arg_short_output_to_file);

    if (files.size() > 1)
        throw std::runtime_error("Too much files for output!");

    bool junit_requested = contains(args, arg_junit_output, arg_short_junit_output);
    bool cerr = contains(args, arg_cerr, arg_short_cerr);
    bool use_tiny_mode = contains(args, arg_tiny_mode, arg_short_tiny_mode);
    std::ostream *output_stream = cerr ? &std::cerr : &std::cout;
#if defined(_WIN32) || defined(_WIN64)
    bool use_win_debug = contains(args, arg_windebug_mode, arg_short_windebug_mode);
#endif

    if (!files.empty())
    {
        output_file.open(*files.begin(), std::ofstream::out | std::ofstream::trunc);
        output_stream = &output_file;
    }

    if (junit_requested)
        output = std::make_shared<junit_output>(output_stream);
#if defined(_WIN32) || defined(_WIN64)
	else if (use_win_debug)
        output = std::make_shared<windebug_output>(false, use_tiny_mode);
#endif
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
        test_class_statistics stats_cumulated;

		std::for_each(std::begin(classes), std::end(classes), 
			[&](const test_class &tc)
			{
				if (filter_classes.empty() || (filter_classes.find(tc.first) != std::end(filter_classes)))
                {
					test_class_statistics stats_current_test = tc.second();
					stats_cumulated += stats_current_test;
                    tc_count++;
                }
            }
		);
		output->end(tc_count, stats_cumulated);
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
#if defined(_WIN32) || defined(_WIN64)
	std::cout << "-w, --windebug    ... send output to windows debugger (OutputDebugString)\n";
#endif
	std::cout << std::endl;
}

filter_type extract_filter(const stringlist &args, const std::string &filter_command, const std::string &short_filter_command)
{
    auto start_filter = locate_command(args, filter_command, short_filter_command);
    if (start_filter != std::end(args))
    {
        auto start_filter_args = start_filter + 1;
        auto end_filter = std::find_if(start_filter_args, std::end(args), [](const std::string &arg){return arg.substr(0,1) == arg_short_command_prefix;});
        return filter_type(start_filter_args, end_filter);
    }
    return filter_type();
}

}

