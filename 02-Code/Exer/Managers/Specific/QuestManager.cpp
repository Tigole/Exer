#include "QuestManager.hpp"
#include <sstream>
#include <fstream>
#include "../../Context.hpp"

QuestManager::QuestManager() : ResourceManager<Quest>("Quests")
{}

bool QuestManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Quest>& resource)
{
    bool l_b_Ret;
    std::stringstream l_Stream;
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File))
    {
        l_b_Ret = true;
    }
    else
    {
        l_b_Ret = false;
    }

    Quest* l_Quest = nullptr;

    if (reference_file == "Main_Quest")
    {
        l_Quest = new Quest_Main;
        l_Quest->m_Quest_Id = "Main_Quest";
    }
    else if (reference_file == "Troll_Quest")
    {
        l_Quest = new Quest_Troll;
        l_Quest->m_Quest_Id = "Troll_Quest";
    }
    else if (reference_file == "Tutoriel")
    {
        l_Quest = new Quest_Tutoriel;
        l_Quest->m_Quest_Id = "Tutoriel";
    }
    else if (reference_file == "Sombres_Manigances")
    {
        l_Quest = new Quest_Dark_Mage;
        l_Quest->m_Quest_Id = "Sombres_Manigances";
    }

    resource.reset(l_Quest);
    l_b_Ret = l_Quest != nullptr;

    return l_b_Ret;
}

void QuestManager::mt_Unload_Resource(Quest* resource)
{}
