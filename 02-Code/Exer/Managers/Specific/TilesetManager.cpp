#include "TilesetManager.hpp"
#include "../../Context.hpp"
#include "tinyxml.h"

TilesetManager::TilesetManager() : ResourceManager<Tileset>("Tileset"){}

bool TilesetManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Tileset>& resource)
{
    std::string l_Ext;
    bool l_b_Ret;
    jaja::fp::File l_File;

    l_Ext = reference_file.substr(reference_file.rfind('.')+1);

    if (l_Ext == "tsx")
    {
        TiXmlDocument l_Doc;

        if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File) && fn_Parse_XML_Document(l_Doc, l_File))
        {
            std::string l_Texture_Name;
            const TiXmlElement* l_Element = l_Doc.RootElement();
            int l_Tile_Count(1);
            std::function<void(const TiXmlElement* Tile_Element)> l_fn_Solid = [&](const TiXmlElement* Tile_Element)
            {
                int l_current_tile(0);
                const TiXmlElement* l_Prop_Element = Tile_Element->FirstChildElement("properties")->FirstChildElement("property");
                std::string l_Prop;

                l_b_Ret &= Tile_Element->QueryIntAttribute("id", &l_current_tile) == TIXML_SUCCESS;

                for (l_Prop_Element = Tile_Element->FirstChildElement("properties")->FirstChildElement("property"); (l_Prop_Element != nullptr) && (l_b_Ret == true); l_Prop_Element = l_Prop_Element->NextSiblingElement("property"))
                {
                    l_b_Ret &= l_Prop_Element->QueryStringAttribute("name", &l_Prop) == TIXML_SUCCESS;

                    if (l_Prop == "Solid")
                    {
                        l_b_Ret &= l_Prop_Element->QueryBoolAttribute("value", &resource->m_Solid[l_current_tile]) == TIXML_SUCCESS;
                    }
                }

                //if (resource->m_Solid[l_current_tile]) std::cout << l_current_tile << " solid\n";
            };

            l_b_Ret = true;

            resource.reset(new Tileset);
            resource->m_Column_Count = 8;
            resource->m_Tile_Size.x = 32;
            resource->m_Tile_Size.y = 32;

            l_b_Ret &= l_Element->QueryIntAttribute("tilewidth", &resource->m_Tile_Size.x) == TIXML_SUCCESS;
            l_b_Ret &= l_Element->QueryIntAttribute("tileheight", &resource->m_Tile_Size.y) == TIXML_SUCCESS;
            l_b_Ret &= l_Element->QueryIntAttribute("columns", &resource->m_Column_Count) == TIXML_SUCCESS;
            l_b_Ret &= l_Element->QueryStringAttribute("name", &l_Texture_Name) == TIXML_SUCCESS;
            l_b_Ret &= l_Element->QueryIntAttribute("tilecount", &l_Tile_Count) == TIXML_SUCCESS;

            resource->m_Texture = Context::smt_Get().m_Textures.mt_Get_Resource(l_Texture_Name);

            resource->m_Raw_Count = l_Tile_Count/resource->m_Column_Count;
            resource->m_Solid = new bool[l_Tile_Count];

            for (l_Element = l_Element->FirstChildElement("tile"); (l_Element != nullptr) && (l_b_Ret == true); l_Element = l_Element->NextSiblingElement("tile"))
            {
                l_fn_Solid(l_Element);
            }
        }
    }
    else
    {
        l_b_Ret = false;
    }

    return l_b_Ret;
}

void TilesetManager::mt_Unload_Resource(Tileset* resource)
{
    Context::smt_Get().m_Textures.mt_Release(resource->m_Texture);
}
