#ifndef _CREATURE_MANAGER_HPP
#define _CREATURE_MANAGER_HPP 1

#include "../ResourceManager.hpp"
#include "../../Dynamics.hpp"

class DynamicManager : public ResourceManager<Dynamic>
{
public:
    DynamicManager();

protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Dynamic>& resource) override;
    void mt_Unload_Resource(Dynamic* resource) override;
};

#endif // _CREATURE_MANAGER_HPP
