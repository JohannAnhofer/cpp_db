#include "type_mismatch_exception.h"
#include <string>

namespace cpp_db
{

type_mismatch_exception::type_mismatch_exception(const std::type_index &src_type, const std::type_index &dest_type)
    : std::runtime_error(std::string("Value type '")
                            +dest_type.name()
                            + std::string("' is not convertible to '")
                            + src_type.name()+"'"
                         )
{
}

}
