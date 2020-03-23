#include "MusicManager.hpp"
#include "../Context.hpp"

MusicManager::MusicManager() : ResourceManager<sf::Music>("Music")
{}

bool MusicManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Music>& resource)
{
    if (m_Files[reference_file] == nullptr)
    {
        m_Files[reference_file].reset(new jaja::fp::File);
    }
    if (m_Files[reference_file]->m_Buffer == nullptr)
    {
        Context::smt_Get().m_Packer.mt_Get_File(reference_file, *m_Files[reference_file]);
    }

    resource.reset(new sf::Music);

    return resource->openFromMemory(m_Files[reference_file]->m_Buffer, m_Files[reference_file]->m_Size);
}

void MusicManager::mt_Unload_Resource(sf::Music* resource)
{
    resource->stop();
}
