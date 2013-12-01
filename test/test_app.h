#ifndef TEST_TESTAPP_H
#define TEST_TESTAPP_H

#include "test_output.h"

#include <vector>
#include <utility>
#include <unordered_set>
#include <string>
#include <iostream>
#include <functional>
#include <memory>
#include <fstream>

namespace test
{

class test_app
{
public:
    test_app(int argc, char *argv[]);

    template<typename T>
	void add_test_class(const std::string &class_name);

    void run();

private:
	using filter_type = std::unordered_set<std::string>;
    using test_class = std::pair<std::string, std::function<test_class_statistics()>>;
    using test_classes = std::vector<test_class>;

    test_classes classes;
	filter_type filter_classes;
	filter_type filter_functions;

private:
    bool help_requested;
    std::shared_ptr<test_output> output;
    std::ofstream output_file;
};

template<typename T>
void test_app::add_test_class(const std::string &class_name)
{
    auto call = [&]()
                {
                    T tc;
                    tc.set_output(output);
                    tc(filter_functions);
                    return tc.get_statistics();
                };
	classes.push_back(std::make_pair(class_name, call));
}

}

#define BEGIN_DECLARE_TEST_APP(init) int main(int argc, char *argv[]) { init(); test::test_app app(argc, argv);
#define DECLARE_TEST_CLASS(class_name) app.add_test_class<class_name>(#class_name);
#define END_DECLARE_TEST_APP(finalize) app.run(); finalize(); return 0; }

#endif // TEST_TESTAPP_H
