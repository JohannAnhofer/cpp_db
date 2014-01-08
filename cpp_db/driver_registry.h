#ifndef CPP_DB_DRIVER_REGISTRY_H
#define CPP_DB_DRIVER_REGISTRY_H

#include <string>
#include <functional>
#include <memory>

namespace cpp_db
{
    struct  driver_interface;

    namespace driver_registry
    {
        void register_driver(const std::string &name, std::function<driver_interface *()> creator);
        void unregister_driver(const std::string &name);
        std::shared_ptr<driver_interface> make_driver(const std::string &name);
    }
}

#endif // CPP_DB_DRIVER_REGISTRY_H
