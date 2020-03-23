#include "ScriptManager.hpp"
#include <sstream>
#include <fstream>
#include "../../Context.hpp"

ScriptManager::ScriptManager() : ResourceManager<Command>("Script")
{}

bool ScriptManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Command>& resource)
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

    return l_b_Ret;
}

void ScriptManager::mt_Unload_Resource(Command* resource)
{}
