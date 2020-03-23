#ifndef _TILESET_MANAGER_HPP
#define _TILESET_MANAGER_HPP 1

#include "../ResourceManager.hpp"
#include "../../Tileset.hpp"

class TilesetManager : public ResourceManager<Tileset>
{
public:
    TilesetManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Tileset>& resource) override;
    void mt_Unload_Resource(Tileset* resource) override;
};

#endif // _TILESET_MANAGER_HPP
