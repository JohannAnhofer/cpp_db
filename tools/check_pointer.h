#ifndef TOOLS_CHECK_POINTER_H
#define TOOLS_CHECK_POINTER_H

#include <stdexcept>
#include <memory>
#include <string>

namespace tools
{

template<typename T>
std::shared_ptr<T> check_pointer(const std::shared_ptr<T> &p, const std::string &msg_if_null)
{
	if (p)
		return p;
	else
		throw std::runtime_error(msg_if_null);
}

}

#endif
