#ifndef _ITEM_HPP
#define _ITEM_HPP 1

#include <string>
#include "Managers/TextureManager.hpp"
#include "SpriteSheet.hpp"

class Creature;

enum class ItemType
{
    Edible,
    Equipment,
    Quest,

    COUNT
};

class Item
{
public:
    Item();
    virtual ~Item();

    virtual bool mt_OnUse(Creature* object) = 0;

    std::string m_Name;
    std::string m_Description;
    Resource<SpriteSheet> m_Sprite;
    sf::Vector2f m_Sprite_Rect;
    int m_Quantity;
    std::string m_Anim_Id;
};

class Item_Quest : public Item
{
public:
    Item_Quest();

    bool mt_OnUse(Creature* object) override;
};

enum class Edible_Type
{
    Life,
    Psy
};

class Item_Edible : public Item
{
public:
    Item_Edible();
    bool mt_OnUse(Creature* object) override;

    Edible_Type m_Type;
    int m_Restore_Max;
    int m_Restore_Min;
};

class Item_Equipment : public Item
{
public:
    Item_Equipment();
    bool mt_OnUse(Creature* object) override;
};

#endif // _ITEM_HPP
