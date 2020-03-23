#include "ShaderManager.hpp"
#include "../Context.hpp"

ShaderManager::ShaderManager() : ResourceManager<sf::Shader>("Shader")
{}

bool ShaderManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Shader>& resource)
{
    jaja::fp::File l_File;
    std::string l_Vert, l_Frag;

    Context::smt_Get().m_Packer.mt_Get_File(reference_file + ".vert", l_File);

    l_Vert = std::string(l_File.m_Buffer, l_File.m_Size);

    Context::smt_Get().m_Packer.mt_Get_File(reference_file + ".frag", l_File);

    l_Frag = std::string(l_File.m_Buffer, l_File.m_Size);

    resource.reset(new sf::Shader);
    return resource->loadFromMemory(l_Vert, l_Frag);
}

void ShaderManager::mt_Unload_Resource(sf::Shader* resource)
{}
