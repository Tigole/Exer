#ifndef _TEXTURE_MANAGER_HPP
#define _TEXTURE_MANAGER_HPP

#include <SFML/Graphics/Texture.hpp>
#include "ResourceManager.hpp"

class TextureManager : public ResourceManager<sf::Texture>
{
public:
    TextureManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Texture>& resource) override;
    void mt_Unload_Resource(sf::Texture* resource) override;
};

#endif // _TEXTURE_MANAGER_HPP
