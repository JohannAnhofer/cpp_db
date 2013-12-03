#include "firebird_driver.h"
#include "sqlite_driver.h"
#include "driver_factory.h"

#include <iostream>

void run_samples();

int main()
{
    cpp_db::driver_factory::instance().register_driver("sqlite", []{return cpp_db::sqlite_driver::create();});
    cpp_db::driver_factory::instance().register_driver("firebird", []{return cpp_db::firebird_driver::create();});

	run_samples();

    return 0;
}

