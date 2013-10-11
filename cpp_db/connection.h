#ifndef CPP_DB_CONNECTION_H
#define CPP_DB_CONNECTION_H

#include <memory>
#include <string>

namespace cpp_db
{

class connection
{
public:
    using handle = std::shared_ptr<void>;

    explicit connection(const std::string &drivername);
	~connection();

	connection(const connection &other) = delete;
	connection &operator=(const connection &other) = delete;

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
