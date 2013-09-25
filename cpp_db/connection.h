#ifndef CPP_DB_CONNECTION_H
#define CPP_DB_CONNECTION_H

#include <memory>
#include <string>

namespace cpp_db
{

class connection
{
public:
    typedef void * handle;

    connection(const std::string &drivername);
	~connection();

	void open(const std::string &database);
	void close();
    bool is_open() const;
    handle get_handle() const;

private:
	struct impl;
	std::unique_ptr<impl> pimpl;
};

}

#endif
