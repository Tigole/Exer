#ifndef _MAP_HPP
#define _MAP_HPP 1

#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include "Tileset.hpp"
#include <memory>

class Dynamic;

enum class InteractionNature
{
    Walking,
    Talking,
};

struct MapDynamics
{
    std::string m_Id;
    sf::Vector2f m_Pos;
};

class IMapInteration
{
public:
    virtual ~IMapInteration(){}
    virtual void mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature) = 0;
};

class MapInteration_Dialog : public IMapInteration
{
public:
    MapInteration_Dialog(const std::vector<sf::String>& dialog);
    void mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature) override;

    std::vector<sf::String> m_Dialog;
};

class MapInteration_Quest : public IMapInteration
{
public:
    MapInteration_Quest(const std::string& quest_id, IMapInteration* over_interaction);
    void mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature) override;

    std::string m_Quest_Id;
    std::unique_ptr<IMapInteration> m_Over_Interaction;
};

class Map
{
public:
    Map();
    virtual ~Map();
    int m_Width;
    int m_Height;
    std::string m_Name;
    Resource<Tileset> m_Tileset;
    std::vector<MapDynamics> m_Dyns;

    int mt_Get_Index(int x, int y, int lvl);
    bool mt_Is_Solid(int x, int y);

    bool mt_Create(const std::string& file, const std::string& tileset_id, const std::string& name);
    virtual bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns) = 0;

    virtual bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature) = 0;

private:
    int* m_Ground_Indices;
    int* m_Middle_Indices;
    int* m_Air_Indices;
    bool* m_Solids;
    void mt_Delete(void);
};

class Map1 : public Map
{
public:
    Map1();

    bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns) override;
    bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, InteractionNature nature) override;

    std::unordered_map<std::string, std::unique_ptr<IMapInteration>> m_Interations;
};

#endif // _MAP_HPP
