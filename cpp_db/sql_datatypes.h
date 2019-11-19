#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <variant>
#include <vector>

namespace cpp_db
{

struct SqlDate
{
    std::int16_t year = 1971;
    std::int8_t month = 1;
    std::int8_t day = 1;
};

struct SqlTime
{
    std::int8_t hour = 0;
    std::int8_t minute = 0;
    std::int8_t second = 0;
    std::uint16_t milliseconds = 0;
};

struct SqlDateTime
{
    SqlDate date;
    SqlTime time;
};

// missing bit, decimal and numeric
using sql_column_type = std::variant
    <
        std::monostate,             // null
        std::int8_t,                // tinyint
        std::int16_t,               // smallint, year
        std::int32_t,               // int
        std::int64_t,               // bigint, timestamp
        double,                     // float
        float,                      // real
        SqlDate,                    // date
        SqlTime,                    // time
        SqlDateTime,                // datetime
        std::string,                // char, varchar, varchar(max), text
        std::wstring,               // nchar, nvarchar, nvarchar(max), ntext, xml, json
        std::vector<std::int8_t>    // binary, varbinary, varbinary(max), image, clob, blob
    >;

using sal_columns = std::vector<sql_column_type>;

}
