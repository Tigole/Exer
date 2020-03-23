#include "Map.hpp"
#include "Context.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include "tinyxml.h"
#include "GameEngine.hpp"

Map::Map() : m_Width(0), m_Height(0), m_Name(), m_Ground_Indices(nullptr), m_Middle_Indices(nullptr), m_Air_Indices(nullptr), m_Solids(nullptr), m_Tileset()
{}

Map::~Map()
{
    mt_Delete();
}

int Map::mt_Get_Index(int x, int y, int lvl)
{
    if (    (lvl >= 0 && lvl <= 2)
        &&  (x >= 0 && x < m_Width)
        &&  (y >= 0 && y < m_Height))
    {
        if ((lvl == 0) && (m_Ground_Indices != nullptr))
            return m_Ground_Indices[y*m_Width + x];
        if ((lvl == 1) && (m_Middle_Indices != nullptr))
            return m_Middle_Indices[y*m_Width + x];
        if ((lvl == 2) && (m_Air_Indices != nullptr))
            return m_Air_Indices[y*m_Width + x];
        return -1;
    }
    return -1;
}

bool Map::mt_Is_Solid(int x, int y)
{
    if ((x >= 0 && x < m_Width) && (y >= 0 && y < m_Height))
    {
        if (m_Ground_Indices[y*m_Width + x] != -1)
            if (m_Tileset->m_Solid[m_Ground_Indices[y*m_Width + x]])
                return true;
        if (m_Middle_Indices[y*m_Width + x] != -1)
            if (m_Tileset->m_Solid[m_Middle_Indices[y*m_Width + x]])
                return true;
        if (m_Air_Indices[y*m_Width + x] != -1)
            if (m_Tileset->m_Solid[m_Air_Indices[y*m_Width + x]])
                return true;
        return false;
        /*if (m_Solids[y*m_Width + x])
            std::cout << ' ' << x << ' ' << y << " solid from map\n";*/
        return m_Solids[y*m_Width + x];
    }
    return true;
}

bool Map::mt_Create(const std::string& file, const std::string& tileset_id, const std::string& name)
{
    bool l_Ret;
    TiXmlDocument l_Doc;
    jaja::fp::File l_File;

    mt_Delete();
    m_Tileset = Context::smt_Get().m_Tilesets.mt_Get_Resource(tileset_id);
    m_Name = name;

    if (Context::smt_Get().m_Packer.mt_Get_File(file, l_File) && fn_Parse_XML_Document(l_Doc, l_File))
    {
        l_Ret = true;
        TiXmlElement* l_Element = l_Doc.RootElement();
        TiXmlHandle l_handle(l_Element);
        std::function<void(int*)> l_fn_Load_Layer = [&](int* loaded_array)
        {
            std::string l_str;
            std::stringstream l_ss;
            int l_current_index;

            l_ss.str(l_Element->GetText());

            l_current_index = 0;

            while(std::getline(l_ss, l_str, ','))
            {
                int l_number;
                if (l_str.find('\n') != std::string::npos)
                {
                    //std::cout << "Found\n";
                }
                else
                {
                    l_number = std::atoi(l_str.c_str());

                    loaded_array[l_current_index] = l_number-1;
                    m_Solids[l_current_index] |= m_Tileset->mt_Is_Solid(l_current_index);
                    /*if (m_Solids[l_current_index])
                        std::cout << l_current_index << " solid map\n";*/
                    l_current_index++;
                }
            }
        };

        l_Ret &= l_Element->QueryIntAttribute("width", &m_Width) == TIXML_SUCCESS;
        l_Ret &= l_Element->QueryIntAttribute("height", &m_Height) == TIXML_SUCCESS;
        m_Solids = new bool[m_Width * m_Height];
        for (int ii = 0; ii < (m_Width * m_Height); ii++)
            m_Solids[ii] = false;

        l_Element = l_handle.ChildElement("layer", 0).ChildElement("data", 0).ToElement();
        l_Ret &= l_Element != nullptr;

        if (l_Ret == true)
        {
            m_Ground_Indices = new int[m_Width * m_Height];
            l_fn_Load_Layer(m_Ground_Indices);
        }
        l_Element = l_handle.ChildElement("layer", 1).ChildElement("data", 0).ToElement();
        if (l_Element != nullptr)
        {
            m_Middle_Indices = new int[m_Width * m_Height];
            l_fn_Load_Layer(m_Middle_Indices);
        }
        l_Element = l_handle.ChildElement("layer", 2).ChildElement("data", 0).ToElement();
        if (l_Element != nullptr)
        {
            m_Air_Indices = new int[m_Width * m_Height];
            l_fn_Load_Layer(m_Air_Indices);
        }
        l_Element = l_handle.ChildElement("objectgroup", 1).ChildElement("object", 0).ToElement();
        for (; l_Element != nullptr; l_Element = l_Element->NextSiblingElement())
        {
            MapDynamics l_Dyn;

            l_Element->QueryStringAttribute("name", &l_Dyn.m_Id);
            l_Element->QueryFloatAttribute("x", &l_Dyn.m_Pos.x);
            l_Element->QueryFloatAttribute("y", &l_Dyn.m_Pos.y);

            l_Dyn.m_Pos = m_Tileset->mt_Pixel_To_Cell(l_Dyn.m_Pos);

            l_Dyn.m_Pos.x = (int)l_Dyn.m_Pos.x;
            l_Dyn.m_Pos.y = (int)l_Dyn.m_Pos.y;

            m_Dyns.push_back(l_Dyn);
        }

        l_Element = l_handle.ChildElement("objectgroup", 2).ChildElement("object", 0).ToElement();
        for (; l_Element != nullptr; l_Element = l_Element->NextSiblingElement())
        {
            MapDynamics l_Dyn;

            l_Element->QueryStringAttribute("name", &l_Dyn.m_Id);
            l_Element->QueryFloatAttribute("x", &l_Dyn.m_Pos.x);
            l_Element->QueryFloatAttribute("y", &l_Dyn.m_Pos.y);

            l_Dyn.m_Pos = m_Tileset->mt_Pixel_To_Cell(l_Dyn.m_Pos);

            l_Dyn.m_Pos.x = (int)l_Dyn.m_Pos.x;
            l_Dyn.m_Pos.y = (int)l_Dyn.m_Pos.y;

            m_Dyns.push_back(l_Dyn);
        }

        int cpt(0);
        for (int ii = 0; ii < (m_Width * m_Height); ii++)
            if (m_Solids[ii])
                cpt++;

        std::cout << "Solid count: " << cpt << '\n';
    }
    else
    {
        l_Ret = false;
    }

    return l_Ret;
}

void Map::mt_Delete(void)
{
    if (m_Ground_Indices != nullptr)
    {
        delete[] m_Ground_Indices;
        m_Ground_Indices = nullptr;
    }
    if (m_Middle_Indices != nullptr)
    {
        delete[] m_Middle_Indices;
        m_Middle_Indices = nullptr;
    }
    if (m_Air_Indices != nullptr)
    {
        delete[] m_Air_Indices;
        m_Air_Indices = nullptr;
    }
    if (m_Solids != nullptr)
    {
        delete[] m_Solids;
        m_Solids = nullptr;
    }
    if (m_Tileset != nullptr)
    {
        Context::smt_Get().m_Tilesets.mt_Release(m_Tileset);
        m_Tileset = nullptr;
    }
}



#include "Dynamics.hpp"



MapInteration_Dialog::MapInteration_Dialog(const std::vector<sf::String>& dialog)
 :  m_Dialog(dialog)
{}

void MapInteration_Dialog::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature)
{
    Context::smt_Get().m_Engine->m_Script.mt_Add_Command(new Command_ShowDialog(m_Dialog));
}

MapInteration_Quest::MapInteration_Quest(const std::string& quest_id, IMapInteration* over_interaction)
 :  m_Quest_Id(quest_id),
    m_Over_Interaction(over_interaction)
{}

void MapInteration_Quest::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature)
{
    SystemQuest* l_Sys_Quest = &Context::smt_Get().m_Engine->m_System_Quest;
    if (l_Sys_Quest->mt_Is_Quest_Ended(m_Quest_Id) == true)
    {
        if (m_Over_Interaction != nullptr)
            m_Over_Interaction->mt_On_Interaction(dyns, tgt, nature);
    }
    else if (l_Sys_Quest->mt_Has_Quest(m_Quest_Id) == false)
    {
        Quest* l_Quest = l_Sys_Quest->mt_Add_Quest(m_Quest_Id);

        l_Quest->mt_On_Interaction(dyns, tgt, Quest_Nature::Talking);
    }
}



Map1::Map1()
{
    m_Interations.emplace("Voyageur1", std::unique_ptr<IMapInteration>(new MapInteration_Dialog({"Ce troll nous empêche de passer...", "Aidez-nous...", "S'il vous plait..."})));
    m_Interations.emplace("Voyageur2", std::unique_ptr<IMapInteration>(new MapInteration_Dialog({"Bah voilà, on est bloqué !", "Encore..."})));
    m_Interations.emplace("Troll", std::unique_ptr<IMapInteration>(new MapInteration_Quest("Troll_Quest", new MapInteration_Dialog({"Troll:\nMerci!\nTu peux passer."}))));
    m_Interations.emplace("Troll2", std::unique_ptr<IMapInteration>(new MapInteration_Quest("Troll_Quest", nullptr)));
    m_Interations.emplace("Tutoriel_Wolf_Warn1", std::unique_ptr<IMapInteration>(new MapInteration_Quest("Tutoriel", nullptr)));
    m_Interations.emplace("SombreManigances_Villageois", std::unique_ptr<IMapInteration>(new MapInteration_Quest("Sombres_Manigances", nullptr)));
    m_Interations.emplace("SombreManigances_Mage", std::unique_ptr<IMapInteration>(new MapInteration_Quest("Sombres_Manigances", nullptr)));
}

bool Map1::mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns)
{
    for (auto& d : m_Dyns)
    {
        Resource<Dynamic> l_Dyn = Context::smt_Get().m_Dynamics.mt_Get_Resource(d.m_Id);
        l_Dyn->m_Pos = d.m_Pos;
        dyns.push_back(l_Dyn);
    }

    return true;
}

bool Map1::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature)
{
    bool l_b_Ret;
    Teleport* l_Tel = dynamic_cast<Teleport*>(tgt);
    SystemQuest& l_Sys_Quest = Context::smt_Get().m_Engine->m_System_Quest;
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;

    if (l_Tel != nullptr)
    {
        l_Script->mt_Add_Command(new Command_SwitchMap(l_Tel->m_Map_Id, l_Tel->m_Map_Pos));
    }

    auto l_it = m_Interations.find(tgt->m_Name);
    if (l_it != m_Interations.end())
    {
        Creature* l_Creature = dynamic_cast<Creature*>(tgt);

        if (l_Creature != nullptr)
            l_Creature->mt_LookAt(*Context::smt_Get().m_Engine->m_Player);

        l_it->second->mt_On_Interaction(dyns, tgt, nature);
    }

    return l_b_Ret;
}
