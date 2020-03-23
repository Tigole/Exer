#include "SkillManager.hpp"
#include <fstream>
#include <sstream>
#include "../../Context.hpp"

SkillManager::SkillManager() : ResourceManager<ISkill>("Skill") {}

bool SkillManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<ISkill>& resource)
{
    bool l_b_Ret;
    std::stringstream l_Stream;
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File))
    {
        std::string l_Line;
        Skill* l_Skill = new Skill;

        l_Stream.str(l_File.mt_To_String());

        while (fn_Get_Line(l_Stream, l_Line))
        {
            std::stringstream l_ss(l_Line);

            std::getline(l_ss, l_Line, ' ');

            if (l_Line == "NAME")
            {
                std::string l_tmp;
                std::getline(l_ss, l_tmp, '\n');
                l_Skill->m_Name = l_tmp;
            }
            else if (l_Line == "ANIM")
            {
                l_ss >> l_Skill->m_Animation_Id;
            }
            else if (l_Line == "TGT")
            {
                l_ss >> l_Skill->m_Tgt_Max_Count;
            }
            else if (l_Line == "DIST")
            {
                l_ss >> l_Skill->m_Distance_Max;
            }
            else if (l_Line == "DAMAGE")
            {
                l_ss >> l_Skill->m_Damage;
            }
            else if (l_Line == "DESC")
            {
                std::string l_tmp;
                std::getline(l_ss, l_tmp, '\n');
                l_Skill->m_Description = l_tmp;
            }
            else if (l_Line == "COST")
            {
                l_ss >> l_Skill->m_Cost;
            }
            else if (l_Line == "SPRITE")
            {
                std::string l_Sprite_Id;

                l_ss >> l_Sprite_Id >> l_Skill->m_Sprite_Rect.x >> l_Skill->m_Sprite_Rect.y;

                l_Skill->m_Sprite = Context::smt_Get().m_SpriteSheets.mt_Get_Resource(l_Sprite_Id);
            }
        }

        resource.reset(l_Skill);

        l_b_Ret = true;
    }
    else
    {
        l_b_Ret = false;
    }

    return l_b_Ret;
}

void SkillManager::mt_Unload_Resource(ISkill* resource)
{
    //
}
