#include "SystemSound.hpp"
#include "../Context.hpp"

#include <iostream>


const std::string SystemSound::m_Cancel_String = "Cancel";
const std::string SystemSound::m_Validate_String = "Validate";
const std::string SystemSound::m_Buzz_String = "Buzz";
const std::string SystemSound::m_Chest_String = "Chest";


void SystemSound::mt_Play_Sound(const std::string& sound_id, const sf::Vector3f& pos, bool relative)
{
    std::size_t ii = 0;

    for (ii = 0; (ii < m_Sounds.size()) && (m_Sounds[ii].getStatus() == sf::SoundSource::Playing); ii++)
    {
        std::cout << (int)m_Sounds[ii].getStatus() << '\n';
    }

    if (ii < m_Sounds.size())
    {
        Context::smt_Get().m_Sounds.mt_Release(m_Buffers[ii]);
        m_Buffers[ii] = Context::smt_Get().m_Sounds.mt_Get_Resource(sound_id);
        m_Sounds[ii].setBuffer(*m_Buffers[ii].m_Resource);
        m_Sounds[ii].setRelativeToListener(relative);
        m_Sounds[ii].setPosition(pos);
        m_Sounds[ii].play();
    }
}

