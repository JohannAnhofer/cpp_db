#pragma once

#include <typeinfo>
#include <typeindex>

namespace cpp_db
{

template<typename T>
std::type_index type_of(const T &)
{
	return typeid(T);
}

}
