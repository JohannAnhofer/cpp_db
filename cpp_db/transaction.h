#ifndef CPP_DB_TRANSACTION
#define CPP_DB_TRANSACTION

#include <memory>

namespace cpp_db
{

class connection;

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
	struct impl;
	std::unique_ptr<impl> pimpl;
};

}

#endif
