#ifndef _SKILL_MANAGER_HPP
#define _SKILL_MANAGER_HPP 1

#include "../ResourceManager.hpp"
#include "../../Skill.hpp"

class SkillManager : public ResourceManager<ISkill>
{
public:
    SkillManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<ISkill>& resource) override;
    void mt_Unload_Resource(ISkill* resource) override;
};

#endif // _SKILL_MANAGER_HPP
