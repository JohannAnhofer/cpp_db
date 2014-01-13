#include "driver_factory.h"

#include <utility>
#include <stdexcept>

namespace cpp_db
{

driver_factory &driver_factory::instance()
{
    static driver_factory factory;
    return factory;
}

void driver_factory::register_driver(const std::string &name, driver_factory::driver_creator_function creator)
{
    std::lock_guard<std::mutex> lg(factory_mutex);
    drivers.insert(std::make_pair(name, creator));
}

void driver_factory::unregister_driver(const std::string &name)
{
    std::lock_guard<std::mutex> lg(factory_mutex);
    auto pos = drivers.find(name);
    drivers.erase(pos, pos);
}

std::shared_ptr<driver_interface> driver_factory::create_driver(const std::string &name) const
{
    std::lock_guard<std::mutex> lg(factory_mutex);
    auto pos = drivers.find(name);
    if (pos != std::end(drivers))
        return std::shared_ptr<driver_interface>(pos->second());
    throw std::runtime_error("Driver not registered: " + name);
}

std::vector<std::string> driver_factory::registered_drivers() const
{
    std::lock_guard<std::mutex> lg(factory_mutex);
    std::vector<std::string> names;
    for(const auto &driver: drivers)
        names.push_back(driver.first);
    return names;
}

}
