#ifndef CPP_DB_TYPE_OF_H
#define CPP_DB_TYPE_OF_H

#include <typeinfo>
#include <typeindex>

#include "cpp11_defines.h"

namespace cpp_db
{

template<typename T>
CONSTEXPR std::type_index type_of(const T &)
{
	return typeid(T);
}

}

#endif // CPP_DB_TYPE_OF_H
