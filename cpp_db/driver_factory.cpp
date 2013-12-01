#include "driver_factory.h"

#include <utility>

namespace cpp_db
{

driver_factory &driver_factory::instance()
{
    static driver_factory factory;
    return factory;
}

void driver_factory::register_driver(const std::string &name, driver_factory::driver_creator_function creator)
{
    drivers.insert(std::make_pair(name, creator));
}

void driver_factory::unregister_driver(const std::string &name)
{
    auto pos = drivers.find(name);
    drivers.erase(pos, pos);
}

std::shared_ptr<driver_interface> driver_factory::create_driver(const std::string &name) const
{
    auto pos = drivers.find(name);
    if (pos != std::end(drivers))
        return std::shared_ptr<driver_interface>(pos->second());
    return std::shared_ptr<driver_interface>{};
}

std::vector<std::string> driver_factory::registered_drivers() const
{
    std::vector<std::string> names;
    for(const auto &driver: drivers)
        names.push_back(driver.first);
    return names;
}

}
