#include "SpriteSheetManager.hpp"
#include "../../_Misc.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

SpriteSheetManager::SpriteSheetManager()
 :  ResourceManager<SpriteSheet>("SpriteSheet")
{}

bool SpriteSheetManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<SpriteSheet>& resource)
{
    resource.reset(new SpriteSheet);

    return resource->mt_Load_From_File(reference_file);
}

void SpriteSheetManager::mt_Unload_Resource(SpriteSheet* resource)
{
    //
}

SpriteSheetAnimatorManager::SpriteSheetAnimatorManager()
 :  ResourceManager<SpriteSheetAnimator>("SpriteSheetAnimator")
{}

bool SpriteSheetAnimatorManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<SpriteSheetAnimator>& resource)
{
    resource.reset(new SpriteSheetAnimator);

    return resource->mt_Load_From_File(reference_file);
}

void SpriteSheetAnimatorManager::mt_Unload_Resource(SpriteSheetAnimator* resource)
{
    //
}
