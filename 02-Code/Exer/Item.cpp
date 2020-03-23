#include "Item.hpp"
#include "Dynamics.hpp"

Item::Item()
 :  m_Name(""),
    m_Description(""),
    //m_Texture(),
    m_Quantity(0)
{}

Item::~Item(){}




Item_Quest::Item_Quest()
{}

bool Item_Quest::mt_OnUse(Creature* object)
{
    return false;
}




Item_Edible::Item_Edible()
{}

bool Item_Edible::mt_OnUse(Creature* object)
{
    int l_Res;

    if (m_Restore_Min != m_Restore_Max)
    {
        l_Res = std::rand() % (m_Restore_Max - m_Restore_Min);
        l_Res += m_Restore_Min;
    }
    else
    {
        l_Res = m_Restore_Min;
    }

    if (m_Type == Edible_Type::Life)
    {
        object->m_Gameplay_Data.m_Health = std::min(object->m_Gameplay_Data.m_Health + l_Res, object->m_Gameplay_Data.m_Health_Max);
        m_Quantity--;
    }
    else if (m_Type == Edible_Type::Psy)
    {
        object->m_Gameplay_Data.m_Data_Aventure.m_Psy = std::min(object->m_Gameplay_Data.m_Data_Aventure.m_Psy + l_Res, object->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max);
        m_Quantity--;
    }

    return true;
}

