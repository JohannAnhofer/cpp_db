#ifndef USER_PASSWORD_AUTHENTICATION_H
#define USER_PASSWORD_AUTHENTICATION_H

#include "authentication.h"

#include <string>

namespace cpp_db
{

class user_password_authentication : public authentication
{
public:
	user_password_authentication(const std::string &user, const std::string &pwd)
		: user_name_(user)
		, password_(pwd)
	{
	}

	user_password_authentication(const user_password_authentication &) = delete;
	user_password_authentication &operator = (const user_password_authentication &) = delete;

    virtual std::string user_name() const
	{
		return user_name_;
	}

    virtual std::string password() const
	{
		return password_;
	}

private:
	std::string user_name_;
	std::string password_;
};

}

#endif
