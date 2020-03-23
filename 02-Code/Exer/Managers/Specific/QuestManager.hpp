#ifndef _QUEST_MANAGER_HPP
#define _QUEST_MANAGER_HPP 1

#include "../ResourceManager.hpp"
#include "../../Quest.hpp"

class QuestManager : public ResourceManager<Quest>
{
public:
    QuestManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Quest>& resource) override;
    void mt_Unload_Resource(Quest* resource) override;
};

#endif // _QUEST_MANAGER_HPP
