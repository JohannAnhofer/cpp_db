#ifndef CPP_DB_AUTHENTICATION_H
#define CPP_DB_AUTHENTICATION_H

namespace cpp_db
{

class authentication
{
public:
	virtual ~authentication() = 0;
};

inline authentication::~authentication()
{
}

}

#endif
