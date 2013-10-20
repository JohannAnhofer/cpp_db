#ifndef TEST_TESTAPP_H
#define TEST_TESTAPP_H

#include <map>
#include <string>
#include <iostream>

namespace test
{

class test_app
{
public:
    test_app(int argc, char *argv[]) {(void)argc;(void)argv;}

    template<typename T>
    void add_test_class(const std::string &class_name)
    {
        classes[class_name] = [](){T tc; tc();};
    }

    void run()
    {
        for(auto test_class: classes)
            test_class.second();
    }

private:
    using test_classes = std::map<std::string, void (*)()>;
    test_classes classes;
};

}

#define BEGIN_DECLARE_TEST_APP() int main(int argc, char *argv[]) \
{ \
    test::test_app app(argc, argv);

#define DECLARE_TEST_CLASS(class_name) app.add_test_class<class_name>(#class_name);

#define END_DECLARE_TEST_APP() std::cout << std::endl; \
    app.run(); \
    return 0; \
}

#endif // TEST_TESTAPP_H
