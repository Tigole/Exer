#include "MapManager.hpp"
#include "../../Context.hpp"

MapManager::MapManager() : ResourceManager<Map>("Map") {}

bool MapManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Map>& resource)
{
    bool l_b_Ret;
    Map* l_Map = new Map1;
    std::string l_tmx_file;
    std::string l_Map_Id;

    if (reference_file == "Road")
    {
        l_tmx_file = "Assets/Maps/Road.tmx";
        l_Map_Id = "Road";
    }
    else if (reference_file == "Map2")
    {
        l_tmx_file = "Assets/Maps/Map2.tmx";
        l_Map_Id = "Map2";
    }

    l_b_Ret = l_Map->mt_Create(l_tmx_file, "Tileset1", l_Map_Id);

    resource.reset(l_Map);

    return l_b_Ret;
}

void MapManager::mt_Unload_Resource(Map* resource)
{
    Context::smt_Get().m_Tilesets.mt_Release(resource->m_Tileset);
}
