#include "odbc_parameters.h"
#include "parameter.h"

namespace cpp_db
{

odbc_parameters::odbc_parameters(const shared_statement_ptr &stmt_in)
    : stmt(stmt_in)
{
}

odbc_parameters::~odbc_parameters()
{
}

int odbc_parameters::get_count() const
{
    return 0;
}

void odbc_parameters::bind(const parameter &param)
{
    (void)param;
}

handle odbc_parameters::get_handle() const
{
    return handle{};
}

} // namespace cpp_db
