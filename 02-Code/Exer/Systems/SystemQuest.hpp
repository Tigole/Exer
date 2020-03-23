#ifndef _SYSTEM_QUEST_HPP
#define _SYSTEM_QUEST_HPP 1

#include "../Quest.hpp"

class SystemQuest
{
public:
    Quest* mt_Add_Quest(const std::string& quest_id);
    bool mt_Has_Quest(const std::string& quest_id);
    bool mt_Is_Quest_Ended(const std::string& quest_id);
    std::vector<Resource<Quest>> m_Quests;
    std::vector<std::string> m_Ended_Quests;
};


#endif // _SYSTEM_QUEST_HPP
