#include "sqlite_driver.h"
#include "driver_registry.h"

#include <iostream>

void run_samples();

int main()
{
    cpp_db::driver_registry::register_driver("sqlite", []{return cpp_db::sqlite_driver::create();});

	run_samples();

    return 0;
}

