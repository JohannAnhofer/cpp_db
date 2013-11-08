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

struct connection_interface;
struct statement_interface;
struct parameters_interface;
struct result_interface;
struct transaction_interface;

using shared_connection_ptr = std::shared_ptr<connection_interface>;
using shared_statement_ptr = std::shared_ptr<statement_interface>;
using shared_parameters_ptr = std::shared_ptr<parameters_interface>;
using shared_result_ptr = std::shared_ptr<result_interface>;
using shared_transaction_ptr = std::shared_ptr<transaction_interface>;

}

#endif // CPP_DB_USINGS_H
