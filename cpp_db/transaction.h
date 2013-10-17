#ifndef CPP_DB_TRANSACTION
#define CPP_DB_TRANSACTION

#include <memory>

namespace cpp_db
{

class connection;
struct transaction_interface;

class transaction
{
public:
	explicit transaction(const connection &conn);
    ~transaction();

	void begin();
	void commit();
	void rollback();

	bool is_open() const;

private:
	std::unique_ptr<transaction_interface> ptransaction;
};

}

#endif
