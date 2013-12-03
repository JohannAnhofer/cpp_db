#include "type_mismatch.h"
#include <string>

namespace cpp_db
{

type_mismatch::type_mismatch(const std::type_index &src_type, const std::type_index &dest_type)
    : std::runtime_error(std::string("Value type '")
                            +dest_type.name()
                            + std::string("' is not convertible to '")
                            + src_type.name()+"'"
                         )
{
}

}
