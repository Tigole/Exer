#ifndef _SOUND_MANAGER_HPP
#define _SOUND_MANAGER_HPP 1

#include "ResourceManager.hpp"
#include <SFML/Audio/SoundBuffer.hpp>

class SoundManager : public ResourceManager<sf::SoundBuffer>
{
public:
    SoundManager();
    bool mt_Load(const std::string& file_desc) override;
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::SoundBuffer>& resource) override;
    void mt_Unload_Resource(sf::SoundBuffer* resource) override;
};

#endif // _SOUND_MANAGER_HPP
