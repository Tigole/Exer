#include "FontManager.hpp"
#include "../Context.hpp"


FontManager::FontManager() : ResourceManager<sf::Font>("Font")
{}

bool FontManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Font>& resource)
{
    if (m_Files[reference_file] == nullptr)
    {
        m_Files[reference_file].reset(new jaja::fp::File);
    }

    Context::smt_Get().m_Packer.mt_Get_File(reference_file, *m_Files[reference_file]);

    resource.reset(new sf::Font);
    return resource->loadFromMemory(m_Files[reference_file]->m_Buffer, m_Files[reference_file]->m_Size);
}

void FontManager::mt_Unload_Resource(sf::Font* resource)
{}
