#include "ItemManager.hpp"
#include <fstream>
#include <sstream>
#include "../../Context.hpp"

bool ItemFactory::mt_Load(const std::string& file_desc)
{
    jaja::fp::File l_File;
    std::stringstream l_Stream;

    if (Context::smt_Get().m_Packer.mt_Get_File(file_desc, l_File))
    {
        std::string l_Line;

        l_Stream.str(l_File.mt_To_String());

        while(fn_Get_Line(l_Stream, l_Line))
        {
            std::string::size_type l_Separator;
            std::string l_Tmp;
            ItemType l_Type;

            l_Separator = l_Line.find(':', 0);

            l_Tmp = l_Line.substr(0, l_Separator);
            l_Line = l_Line.substr(l_Separator + 1, std::string::npos);
            l_Separator = l_Line.find(':', 0);

            if ((l_Tmp == "Edible") || (l_Tmp == "EDIBLE"))
            {
                l_Type = ItemType::Edible;
            }
            else if ((l_Tmp == "Quest") || (l_Tmp == "QUEST"))
            {
                l_Type = ItemType::Quest;
            }

            l_Tmp = l_Line.substr(0, l_Separator);
            l_Tmp = l_Line.substr(l_Separator + 1, std::string::npos);

            m_Items[l_Type].emplace(l_Line.substr(0, l_Separator), l_Line.substr(l_Separator + 1, std::string::npos));
        }
    }

    return true;
}

Item* ItemFactory::mt_Create_Item(const std::string& item_id, ItemType item_type)
{
    Item* l_Ret(nullptr);
    auto l_it = m_Items.find(item_type);
    std::map<ItemType, bool(ItemFactory::*)(const std::string&, Item*&)> l_Callbacks;
    auto l_Callback_it = l_Callbacks.end();
    auto l_File_it = m_Items[item_type].find(item_id);

    l_Callbacks[ItemType::Edible] = &ItemFactory::mt_Create_Edible;
    l_Callbacks[ItemType::Equipment] = &ItemFactory::mt_Create_Equipment;
    l_Callbacks[ItemType::Quest] = &ItemFactory::mt_Create_Quest;

    if (l_File_it != m_Items[item_type].end())
    {
        l_Callback_it = l_Callbacks.find(item_type);
        if (l_Callback_it != l_Callbacks.end())
        {
            (this->*(l_Callback_it->second))(l_File_it->second, l_Ret);
        }
        else
        {
            throw std::runtime_error("[ItemFactory] Unknown type" + std::to_string((int)item_type));
        }
    }
    else
    {
        throw std::runtime_error("[ItemFactory] Unknown item" + item_id);
    }

    return l_Ret;
}

bool ItemFactory::mt_Create_Edible(const std::string& reference_file, Item*& resource)
{
    std::stringstream l_Stream;
    Item_Edible* l_Item;
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File))
    {
        std::string l_Line;

        l_Stream.str(std::string(l_File.mt_To_String()));

        l_Item = new Item_Edible;

        while(fn_Get_Line(l_Stream, l_Line))
        {
            std::stringstream l_ss(l_Line);

            std::getline(l_ss, l_Line, ' ');
            if (l_Line == "NAME")
            {
                std::getline(l_ss, l_Item->m_Name, '\n');
            }
            else if (l_Line == "DESC")
            {
                std::getline(l_ss, l_Item->m_Description, '\n');
            }
            else if (l_Line == "SPRITE")
            {
                std::string l_Texture_Id;
                l_ss >> l_Texture_Id;
                l_ss >> l_Item->m_Sprite_Rect.x >> l_Item->m_Sprite_Rect.y;

                l_Item->m_Sprite = Context::smt_Get().m_SpriteSheets.mt_Get_Resource(l_Texture_Id);
            }
            else if (l_Line == "MAX")
            {
                l_ss >> l_Item->m_Restore_Max;
            }
            else if (l_Line == "MIN")
            {
                l_ss >> l_Item->m_Restore_Min;
            }
            else if (l_Line == "TYPE")
            {
                l_ss >> (int&)l_Item->m_Type;
            }
            else if (l_Line == "ANIM")
            {
                std::getline(l_ss, l_Item->m_Anim_Id, '\n');
            }
        }

        resource = l_Item;
    }

    return true;
}

bool ItemFactory::mt_Create_Quest(const std::string& reference_file, Item*& resource)
{
    bool l_Ret;
    std::stringstream l_Stream;
    Item_Quest* l_Quest_Item;
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File))
    {
        std::string l_Line;

        l_Stream.str(l_File.mt_To_String());

        l_Quest_Item = new Item_Quest;

        while(fn_Get_Line(l_Stream, l_Line))
        {
            std::stringstream l_ss(l_Line);

            l_ss >> l_Line;
            if (l_Line == "NAME")
            {
                std::getline(l_ss, l_Quest_Item->m_Name, '\n');
            }
            else if (l_Line == "DESC")
            {
                std::getline(l_ss, l_Quest_Item->m_Description, '\n');
            }
        }

        resource = l_Quest_Item;
    }
    else
    {
        throw std::runtime_error("[ItemFactory] Can't open file: " + reference_file);
    }

    return l_Ret;
}


bool ItemFactory::mt_Create_Equipment(const std::string& reference_file, Item*& resource)
{
    bool l_Ret;
    std::stringstream l_Stream;
    Item_Quest* l_Quest_Item;
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File))
    {
        std::string l_Line;

        l_Stream.str(l_File.mt_To_String());

        l_Quest_Item = new Item_Quest;

        while(fn_Get_Line(l_Stream, l_Line))
        {
            std::stringstream l_ss(l_Line);

            l_ss >> l_Line;
            if (l_Line == "NAME")
            {
                std::getline(l_ss, l_Quest_Item->m_Name, '\n');
            }
            else if (l_Line == "DESC")
            {
                std::getline(l_ss, l_Quest_Item->m_Description, '\n');
            }
        }

        resource = l_Quest_Item;
    }
    else
    {
        throw std::runtime_error("[ItemFactory] Can't open file: " + reference_file);
    }

    return l_Ret;
}
