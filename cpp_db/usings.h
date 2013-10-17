#ifndef CPP_DB_USINGS_H
#define CPP_DB_USINGS_H

#include <memory>
#include <vector>
#include <cstdint>

namespace cpp_db
{

using handle = std::shared_ptr<void>;
using blob = std::vector<uint8_t>;

}

#endif // CPP_DB_USINGS_H
