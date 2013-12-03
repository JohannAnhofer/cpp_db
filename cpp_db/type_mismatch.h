#ifndef CPP_DB_TYPE_MISMATCH_H
#define CPP_DB_TYPE_MISMATCH_H

#include <stdexcept>
#include <typeindex>

namespace cpp_db
{

class type_mismatch : public std::runtime_error
{
public:
    type_mismatch(const std::type_index &src_type, const std::type_index &dest_type);
};

}

#endif // CPP_DB_TYPE_MISMATCH_H
