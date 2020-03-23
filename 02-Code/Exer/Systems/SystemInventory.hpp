#ifndef _SYSTEM_INVENTORY_HPP
#define _SYSTEM_INVENTORY_HPP 1

#include "../Item.hpp"
#include <algorithm>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../Context.hpp"


struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};


class SystemInventory
{
public:
    /*void mt_Add_Quest_Item(const std::string& quest_item_id);
    void mt_Remove_Quest_Item(const std::string& quest_item_id);
    bool mt_Has_Quest(const std::string& quest_item_id);*/

    /*void mt_Add_Edible_Item(const std::string& edible_item_id, int quantity);
    Item_Edible* mt_Get_Edible_Item(const std::string& edible_item_id);
    void mt_Next_Edible(std::string& edible_item_id);
    void mt_Previous_Edible(std::string& edible_item_id);

    std::unordered_map<std::string, Resource<Item_Quest>> m_Quest_Items;
    std::unordered_map<std::string, Item_Edible> m_Edible_Items;*/

    Item* mt_Change_Item_Count(std::size_t item_index, int quantity, ItemType item_type)
    {
        Item* l_Ret(nullptr);
        auto l_it = m_Items.find(item_type);

        if (l_it != m_Items.end())
        {
            auto l_it_ret = l_it->second.begin();
            if (item_index < l_it->second.size())
            {
                std::advance(l_it_ret, item_index);

                l_Ret = l_it_ret->second.get();

                l_Ret->m_Quantity += quantity;
            }
        }

        return l_Ret;
    }

    Item* mt_Change_Item_Count(const std::string& item_id, int quantity, ItemType item_type)
    {
        Item* l_Ret(nullptr);
        auto& l_it = m_Items[item_type];
        auto l_snd = l_it.find(item_id);

        if (l_snd == l_it.end())
        {
            l_snd = l_it.emplace(item_id, std::unique_ptr<Item>(Context::smt_Get().m_Items.mt_Create_Item(item_id, item_type))).first;
        }

        l_Ret = l_snd->second.get();
        l_Ret->m_Quantity += quantity;

        if (l_Ret->m_Quantity <= 0)
        {
            l_Ret->m_Quantity = 0;
            l_Ret = nullptr;
        }

        return l_Ret;
    }

    template<typename T>
    T* mt_Get_Item(const std::string& item_id, ItemType item_type)
    {
        T* l_Ret(nullptr);
        auto l_it = m_Items.find(item_type);

        if (l_it != m_Items.end())
        {
            auto l_snd = l_it->second.find(item_id);
            if (l_snd != l_it->second.end())
            {
                l_Ret = l_snd->second.get();
            }
        }

        return l_Ret;
    }
    template<typename T>
    T* mt_Get_Item(std::size_t item_index, ItemType item_type)
    {
        T* l_Ret(nullptr);
        auto l_it = m_Items.find(item_type);

        if (l_it != m_Items.end())
        {
            auto l_it_ret = l_it->second.begin();
            if (item_index >= l_it->second.size())
            {
                item_index = l_it->second.size() - 1;
            }
            std::advance(l_it_ret, item_index);

            l_Ret = dynamic_cast<T*>(l_it_ret->second.get());
        }

        return l_Ret;
    }
    std::size_t mt_Get_Item_Count(ItemType item_type)
    {
        std::size_t l_Ret(0);
        auto l_it = m_Items.find(item_type);

        if (l_it != m_Items.end())
        {
            l_Ret = l_it->second.size();
        }

        return l_Ret;
    }
    int mt_Get_Item_Count(const std::string& item_id, ItemType item_type)
    {
        int l_Ret(0);
        Item* l_Item = mt_Get_Item<Item>(item_id, item_type);

        if (l_Item != nullptr)
        {
            l_Ret = l_Item->m_Quantity;
        }

        return l_Ret;
    }
private:
    std::unordered_map<ItemType, std::map<std::string, std::unique_ptr<Item>>, EnumClassHash> m_Items;
};

#endif // _SYSTEM_INVENTORY_HPP
