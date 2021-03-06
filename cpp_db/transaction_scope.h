#ifndef CPP_DB_TRANSACTION_SCOPE_H
#define CPP_DB_TRANSACTION_SCOPE_H

#include "transaction.h"

#include <stdexcept>

namespace cpp_db
{

class transaction_scope
{
public:
    explicit transaction_scope(transaction *tr)
        : _transaction(tr)
    {
        if (!tr)
            throw std::runtime_error("No transaction object!");
        else
        {
            if (!tr->is_open())
                _transaction->begin();
        }
    }

    ~transaction_scope()
    {
		try 
		{
			if (std::uncaught_exceptions())
				_transaction->rollback();
			else
				_transaction->commit(); 
		}
		catch(...)
		{
		}
    }

private:
    transaction *_transaction;
};

}

#endif // CPP_DB_TRANSACTION_SCOPE_H
