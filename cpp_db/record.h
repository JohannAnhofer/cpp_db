#ifndef CPP_DB_RECORD_H
#define CPP_DB_RECORD_H

#include "value.h"

#include <memory>
#include <string>

namespace cpp_db
{

class statement;

class record
{
public:
    explicit record(const statement &stmt);
	~record();

	record(const record &) = delete;
	record &operator=(const record &) = delete;

	int get_column_count() const;

	bool is_eof() const;

	void move_first();
	void move_next();
	void move_prev();

	value get_column_value(int column) const;
	value get_column_value(const std::string &column_name) const;

	std::string get_column_name(int column) const;
	int get_column_index(const std::string &column_name) const;

private:
	struct impl;
	std::unique_ptr<impl> pimpl;
};

}

#endif
