#include "postgres_parameters.h"
#include "postgres_statement.h"
#include "parameter.h"

namespace cpp_db
{

postgres_parameters::postgres_parameters(const shared_statement_ptr &stmt_in)
    : stmt(stmt_in)
{
}

int postgres_parameters::get_count() const
{
    return 0;
}

void postgres_parameters::bind(const parameter &param)
{
    (void)param;
}

handle postgres_parameters::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
