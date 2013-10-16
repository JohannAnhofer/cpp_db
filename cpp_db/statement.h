#ifndef CPP_DB_STATEMENT_H
#define CPP_DB_STATEMENT_H

#include "parameter.h"
#include "parameters.h"
#include "value.h"
#include "result.h"

#include <memory>
#include <string>

namespace cpp_db
{

class connection;

class statement
{
public:
    using handle = std::shared_ptr<void>;

    statement(const std::string &sqlcmd, connection &conn);
    explicit statement(const connection &conn);
    ~statement();

    statement(const statement&other) = delete;
    statement &operator=(const statement &other) = delete;

    void prepare(const std::string &sqlcmd);
	bool is_prepared() const;
	handle get_handle() const;

	void reset();

	void execute_non_query();
	value execute_scalar();
    result execute();

    template<typename ...Args> void execute_non_query(Args&& ...args);
    template<typename ...Args> value execute_scalar(Args&& ...args);
	template<typename ...Args> result execute(Args&& ...args);

	parameters get_parameters() const;

private:
    struct impl;
    std::unique_ptr<impl> pimpl;
};

}

#include "statement.inl.h"

#endif // CPP_DB_STATEMENT_H
