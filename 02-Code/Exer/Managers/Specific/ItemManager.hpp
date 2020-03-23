#ifndef _ITEM_MANAGER_HPP
#define _ITEM_MANAGER_HPP 1

#include <map>
#include "../ResourceManager.hpp"
#include "../../Item.hpp"

class ItemFactory
{
public:
    bool mt_Load(const std::string& file_desc);

    Item* mt_Create_Item(const std::string& item_id, ItemType item_type);

private:
    bool mt_Create_Edible(const std::string& reference_file, Item*& resource);
    bool mt_Create_Quest(const std::string& reference_file, Item*& resource);
    bool mt_Create_Equipment(const std::string& reference_file, Item*& resource);

    std::map<ItemType, std::map<std::string, std::string>> m_Items;
    /*std::map<std::string, std::string> m_Edible_Items;
    std::map<std::string, std::string> m_Quest_Items;*/
};

#endif // _ITEM_MANAGER_HPP
