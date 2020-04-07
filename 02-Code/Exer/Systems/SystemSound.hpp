#ifndef _SYSTEM_SOUND_HPP
#define _SYSTEM_SOUND_HPP 1

#include <SFML/Audio.hpp>
#include <array>
#include "../Managers/SoundManager.hpp"

class SystemSound
{
public:
    float mt_Play_Sound(const std::string& sound_id, const sf::Vector3f& pos, bool relative);

    static const std::string m_Cancel_String;
    static const std::string m_Validate_String;
    static const std::string m_Buzz_String;
    static const std::string m_Chest_String;

    sf::Listener m_Listener;
    std::array<sf::Sound, 8> m_Sounds;
    std::array<Resource<sf::SoundBuffer>, 8> m_Buffers;
};

#endif // _SYSTEM_SOUND_HPP
