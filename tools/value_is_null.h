#ifndef TOOLS_VALUE_IS_NULL_H
#define TOOLS_VALUE_IS_NULL_H

#include <stdexcept>

namespace tools
{

class value_is_null : public std::logic_error
{
public:
	value_is_null();
};

} // namespace tools

#endif // TOOLS_VALUE_IS_NULL_H
