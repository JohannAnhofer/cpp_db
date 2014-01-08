#include "driver_factory.h"
#include "driver_interface.h"
#include "driver_factory.h"

namespace cpp_db
{

using driver_creator_function = std::function<driver_interface *()>;


namespace driver_registry
{

void register_driver(const std::string &name, driver_creator_function creator)
{
    driver_factory::instance().register_driver(name, creator);
}

void unregister_driver(const std::string &name)
{
    driver_factory::instance().unregister_driver(name);
}

std::shared_ptr<driver_interface> make_driver(const std::string &name)
{
    return driver_factory::instance().create_driver(name);
}

}

}
