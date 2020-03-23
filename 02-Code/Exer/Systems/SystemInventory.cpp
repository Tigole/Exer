#include "SystemInventory.hpp"
#include "../Context.hpp"
/*
void SystemInventory::mt_Add_Quest_Item(const std::string& quest_item_id)
{
    m_Quest_Items.emplace(quest_item_id, Context::smt_Get().m_Items_Quests.mt_Get_Resource(quest_item_id));
}

void SystemInventory::mt_Remove_Quest_Item(const std::string& quest_item_id)
{
    m_Quest_Items.erase(quest_item_id);
}

bool SystemInventory::mt_Has_Quest(const std::string& quest_item_id)
{
    return (m_Quest_Items.find(quest_item_id) != m_Quest_Items.end());
}*/
/*
void SystemInventory::mt_Add_Edible_Item(const std::string& edible_item_id, int quantity)
{
    Item_Edible* l_Item = mt_Get_Edible_Item(edible_item_id);

    if (l_Item == nullptr)
    {
        m_Edible_Items.emplace(edible_item_id, Item_Edible());
        l_Item = mt_Get_Edible_Item(edible_item_id);
    }

    l_Item->m_Quantity += quantity;
}

Item_Edible* SystemInventory::mt_Get_Edible_Item(const std::string& edible_item_id)
{
    Item_Edible* l_Item(nullptr);
    auto l_it = m_Edible_Items.find(edible_item_id);

    if (l_it != m_Edible_Items.end())
    {
        l_Item = &l_it->second;
    }


    return l_Item;
}

void SystemInventory::mt_Next_Edible(std::string& edible_item_id)
{
    //
}

void SystemInventory::mt_Previous_Edible(std::string& edible_item_id)
{
    auto l_it = m_Edible_Items.find(edible_item_id);
    if (l_it == m_Edible_Items.end())
    {
        edible_item_id = m_Edible_Items.begin()->first;
    }
    else
    {
        if (l_it != m_Edible_Items.end())
        {
            l_it;
            edible_item_id = l_it->first;
        }
    }
}*/
