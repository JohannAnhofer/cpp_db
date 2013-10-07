#ifndef CPP_DB_TRANSACTION
#define CPP_DB_TRANSACTION

#include "connection.h"
#include "transaction.h"

namespace cpp_db
{

	class transaction_scope
	{
	public:
		explicit transaction_scope(const connection &con)
			: trans(con)
		{
			trans.begin();
		}

		void cancel()
		{
			trans.rollback();
		}

		~transaction_scope()
		{
			if (trans.is_open())
				trans.commit();
		}

	private:
		transaction trans;
	};

}

#endif
