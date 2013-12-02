#ifndef CPP_DB_TYPE_MISMATCH_EXCEPTION_H
#define CPP_DB_TYPE_MISMATCH_EXCEPTION_H

#include <stdexcept>
#include <typeindex>

namespace cpp_db
{

class type_mismatch_exception : public std::runtime_error
{
public:
    type_mismatch_exception(const std::type_index &src_type, const std::type_index &dest_type);
};

}

#endif // CPP_DB_TYPE_MISMATCH_EXCEPTION_H
