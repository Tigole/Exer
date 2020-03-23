#ifndef _MAP_MANAGER_HPP
#define _MAP_MANAGER_HPP 1

#include "../ResourceManager.hpp"
#include "../../Map.hpp"

class MapManager : public ResourceManager<Map>
{
public:
    MapManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Map>& resource) override;
    void mt_Unload_Resource(Map* resource) override;
};

#endif // _MAP_MANAGER_HPP
