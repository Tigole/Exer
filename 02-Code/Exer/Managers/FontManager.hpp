#ifndef _FONT_MANAGER_HPP
#define _FONT_MANAGER_HPP 1

#include "ResourceManager.hpp"
#include <SFML/Graphics/Font.hpp>
#include "FilePacker/FilePacker.hpp"

class FontManager : public ResourceManager<sf::Font>
{
public:
    FontManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Font>& resource) override;
    void mt_Unload_Resource(sf::Font* resource) override;

    std::map<std::string, std::unique_ptr<jaja::fp::File>> m_Files;
};


#endif // _FONT_MANAGER_HPP
