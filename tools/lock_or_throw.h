#ifndef TOOLS_LOCK_OR_THROW_H
#define TOOLS_LOCK_OR_THROW_H

#include <memory>
#include <stdexcept>

namespace tools
{

template<typename T>
void do_check(const std::weak_ptr<T> &ptr, const char *context)
{
	if (ptr.expired())
		throw std::runtime_error(context ? context : "Invalid pointer!");
}

template<typename T>
std::shared_ptr<T> lock_or_throw(std::weak_ptr<T> &ptr, const char *context = nullptr)
{
	do_check(ptr, context);
    return ptr.lock();
}

template<typename T>
const std::shared_ptr<T> lock_or_throw(const std::weak_ptr<T> &ptr, const char *context = nullptr)
{
	do_check(ptr, context);
	return ptr.lock();
}

}

#endif // TOOLS_LOCK_OR_THROW_H
