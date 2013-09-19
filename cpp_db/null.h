#ifndef CPP_DB_NULL_H
#define CPP_DB_NULL_H

namespace cpp_db
{

struct null_type {};

bool is_null(null_type)
{
    return true;
}

template<typename T>
bool is_null(T &&)
{
    return false;
}

}

#endif // CPP_DB_NULL_H
