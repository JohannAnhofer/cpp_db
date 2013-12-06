#ifndef CPP_DB_TRANSACTION
#define CPP_DB_TRANSACTION

#include "usings.h"

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

#if !defined(_MSC_VER) || (_MSC_FULL_VER > 180021005)
    transaction(transaction && other) = default;
    transaction &operator=(transaction && other) = default;
#else
    transaction(transaction && other);
    transaction &operator=(transaction &&other);
#endif

	void begin();
	void commit();
	void rollback();

	bool is_open() const;

    handle get_handle() const;

private:
    std::weak_ptr<connection_interface> conn_impl;
    std::shared_ptr<transaction_interface> trans_impl;
};

}

#endif
