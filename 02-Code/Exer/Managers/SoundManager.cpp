#include "SoundManager.hpp"
#include "../Systems/SystemSound.hpp"
#include "../Context.hpp"
#include "ResourceManager_Functions.hpp"

SoundManager::SoundManager() : ResourceManager<sf::SoundBuffer>("Sound")
{}

bool SoundManager::mt_Load(const std::string& file_desc)
{
    mt_Add_Resource(SystemSound::m_Validate_String, new ResourceData<sf::SoundBuffer>("Assets/Sounds/Choose.WAV"));
    mt_Add_Resource(SystemSound::m_Cancel_String, new ResourceData<sf::SoundBuffer>("Assets/Sounds/Cancel1.ogg"));
    mt_Add_Resource(SystemSound::m_Buzz_String, new ResourceData<sf::SoundBuffer>("Assets/Sounds/Buzzer1.ogg"));
    mt_Add_Resource(SystemSound::m_Chest_String, new ResourceData<sf::SoundBuffer>("Assets/Sounds/Chest.ogg"));
    return fn_Load_Config(*this, file_desc);
}

bool SoundManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::SoundBuffer>& resource)
{
    jaja::fp::File l_File;

    Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File);

    resource.reset(new sf::SoundBuffer);
    return resource->loadFromMemory(l_File.m_Buffer, l_File.m_Size);
}

void SoundManager::mt_Unload_Resource(sf::SoundBuffer* resource)
{}
