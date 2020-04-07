#ifndef _SYSTEM_QUEST_HPP
#define _SYSTEM_QUEST_HPP 1

#include "../Quest.hpp"

class SystemQuest
{
public:
    Quest* mt_Add_Quest(const std::string& quest_id);
    bool mt_Has_Quest(const std::string& quest_id);
    bool mt_Is_Quest_Ended(const std::string& quest_id);

    template<class Q>
    const Q* mt_Get_Quest(const std::string& quest_id) const
    {
        const Q* l_Ret(nullptr);
        auto l_it = std::find_if(m_Quests.begin(), m_Quests.end(), [&](const Resource<Quest> q){return q->m_Quest_Id == quest_id;});

        if (l_it != m_Quests.end())
        {
            l_Ret = dynamic_cast<Q*>(l_it->m_Resource);
        }

        return l_Ret;
    }

    std::vector<Resource<Quest>> m_Quests;
    //std::vector<std::string> m_Ended_Quests;
};


#endif // _SYSTEM_QUEST_HPP
