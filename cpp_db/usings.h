#ifndef CPP_DB_USINGS_H
#define CPP_DB_USINGS_H

#include <memory>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace cpp_db
{

using handle = std::shared_ptr<void>;
using blob = std::vector<uint8_t>;
using key_value_pair = std::unordered_map<std::string, std::string>;

}

#endif // CPP_DB_USINGS_H
