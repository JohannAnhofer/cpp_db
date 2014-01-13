#ifndef CPP_DB_DRIVER_FACTORY_H
#define CPP_DB_DRIVER_FACTORY_H

#include "driver_interface.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>

namespace cpp_db
{

class driver_factory
{
public:
    driver_factory(const driver_factory &) = delete;
    driver_factory &operator=(const driver_factory &) = delete;

    driver_factory(driver_factory &&) = delete;
    driver_factory &operator=(driver_factory &&) = delete;

    static driver_factory &instance();

    using driver_creator_function = std::function<driver_interface *()>;

    void register_driver(const std::string &name, driver_creator_function creator);
    void unregister_driver(const std::string &name);

    std::shared_ptr<driver_interface> create_driver(const std::string &name) const;

    std::vector<std::string> registered_drivers() const;

private:
    driver_factory() = default;
    std::unordered_map<std::string, driver_creator_function> drivers;
    mutable std::mutex factory_mutex;
};

}

#endif // CPP_DB_DRIVER_FACTORY_H
