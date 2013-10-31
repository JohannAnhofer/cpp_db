#ifndef TEST_FIREBIRD_CLASS_H
#define TEST_FIREBIRD_CLASS_H

#include "abstract_test.h"
#include "connection.h"

#include <memory>

class test_firebird_class : public test::abstract_test
{
public:
	BEGIN_DECLARE_TEST_CLASS(test_firebird_class)
	END_DECLARE_TEST_CLASS()

protected:
	void init_class() override;
	void cleanup_class() override;

private:
	std::unique_ptr<cpp_db::connection> con;
};

#endif // TEST_FIREBIRD_CLASS_H
