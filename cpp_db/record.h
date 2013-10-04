#ifndef CPP_DB_RECORD_H
#define CPP_DB_RECORD_H

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

	std::string get_field_value(int field) const;
	std::string get_field_value(const std::string &field) const;

	std::string get_field_name(int field) const;

private:
	struct impl;
	std::unique_ptr<impl> pimpl;
};

}

#endif
