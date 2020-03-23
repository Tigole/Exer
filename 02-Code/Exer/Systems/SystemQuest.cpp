#include "SystemQuest.hpp"
#include "../Context.hpp"



Quest* SystemQuest::mt_Add_Quest(const std::string& quest_id)
{
    auto l_it = std::find_if(m_Quests.begin(), m_Quests.end(), [&](const Resource<Quest>& q){return q->m_Quest_Id == quest_id;});
    Quest* l_Ret;

    if (l_it == m_Quests.end())
    {
        m_Quests.push_back(Context::smt_Get().m_Quests.mt_Get_Resource(quest_id));
        l_Ret = m_Quests.back().m_Resource;
    }
    else
    {
        l_Ret = l_it->m_Resource;
    }

    return l_Ret;
}

bool SystemQuest::mt_Has_Quest(const std::string& quest_id)
{
    return std::find_if(m_Quests.begin(), m_Quests.end(), [&](const Resource<Quest>& q){return q->m_Quest_Id == quest_id;}) != m_Quests.end();
}

bool SystemQuest::mt_Is_Quest_Ended(const std::string& quest_id)
{
    return std::find_if(m_Ended_Quests.begin(), m_Ended_Quests.end(), [&](const std::string& id){return id == quest_id;}) != m_Ended_Quests.end();
}



