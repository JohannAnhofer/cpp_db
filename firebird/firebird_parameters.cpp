#include "firebird_parameters.h"
#include "isc_status.h"

namespace cpp_db
{


firebird_parameters::firebird_parameters(const handle &stmt_in)
    : stmt(std::static_pointer_cast<isc_stmt_handle>(stmt_in))
    , isqlda{nullptr, free}
{
    allocate_sqlda(1);

    guarded_execute([this](ISC_STATUS *status)
        {
            isc_dsql_describe_bind(status, stmt.lock().get(), 1, isqlda.get());
        }
        , true);

    if (isqlda->sqld > isqlda->sqln) /* Need more XSQLVARs. */
    {
      allocate_sqlda(isqlda->sqld);

      guarded_execute([this](ISC_STATUS *status)
          {
            isc_dsql_describe_bind(status, stmt.lock().get(), 1, isqlda.get());
          }
          , true);
    }
}

int firebird_parameters::get_count() const
{
    return isqlda->sqld;
}

void firebird_parameters::bind(const parameter &param)
{
    (void)param;
}

int firebird_parameters::find_param_pos(const std::string &name) const
{
    (void)name;
    return -1;
}

void firebird_parameters::allocate_sqlda(int vars_count)
{
    isqlda.reset((XSQLDA *)malloc(XSQLDA_LENGTH(vars_count)));
    if (isqlda == nullptr)
        throw std::runtime_error("Can't allocated XSQLDA!");
    isqlda->version = SQLDA_VERSION1;
    isqlda->sqln = vars_count;
}

} // namespace cpp_db
