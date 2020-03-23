#include "TextureManager.hpp"
#include "../Context.hpp"

TextureManager::TextureManager() : ResourceManager<sf::Texture>("Texture")
{}

bool TextureManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<sf::Texture>& resource)
{
    jaja::fp::File l_File;

    Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File);

    resource.reset(new sf::Texture);

    return resource->loadFromMemory(l_File.m_Buffer, l_File.m_Size);
}

void TextureManager::mt_Unload_Resource(sf::Texture* resource)
{}
