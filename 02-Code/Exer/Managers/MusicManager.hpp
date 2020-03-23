#ifndef _MUSIC_MANAGER_HPP
#define _MUSIC_MANAGER_HPP 1

#include "ResourceManager.hpp"
#include <SFML/Audio/Music.hpp>
#include "FilePacker/FilePacker.hpp"
#include <map>

class MusicManager : public ResourceManager<sf::Music>
{
public:
    MusicManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Music>& resource) override;
    void mt_Unload_Resource(sf::Music* resource) override;

    std::map<std::string, std::unique_ptr<jaja::fp::File>> m_Files;
};

#endif // _MUSIC_MANAGER_HPP
