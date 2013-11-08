#ifndef CPP_DB_TRANSACTION
#define CPP_DB_TRANSACTION

#include <memory>

namespace cpp_db
{

class connection;
class driver;
struct transaction_interface;
struct connection_interface;

class transaction
{
public:
	explicit transaction(const connection &conn);
    ~transaction();

	transaction(const transaction &) = delete;
	transaction &operator=(const transaction &) = delete;

	void begin();
	void commit();
	void rollback();

	bool is_open() const;

private:
    std::weak_ptr<connection_interface> conn_impl;
    std::shared_ptr<transaction_interface> trans_impl;
};

}

#endif
