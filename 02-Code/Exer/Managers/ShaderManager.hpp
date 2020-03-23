#ifndef _SHADER_MANAGER_HPP
#define _SHADER_MANAGER_HPP 1

#include "ResourceManager.hpp"
#include <SFML/Graphics/Shader.hpp>

class ShaderManager : public ResourceManager<sf::Shader>
{
public:
    ShaderManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Shader>& resource) override;
    void mt_Unload_Resource(sf::Shader* resource) override;
};

#endif // _SHADER_MANAGER_HPP
