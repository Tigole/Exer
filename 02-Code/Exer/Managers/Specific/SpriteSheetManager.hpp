#ifndef _SPRITE_SHEET_MANAGER_HPP
#define _SPRITE_SHEET_MANAGER_HPP 1

#include "../../SpriteSheet.hpp"
#include "../ResourceManager.hpp"

class SpriteSheetManager : public ResourceManager<SpriteSheet>
{
public:
    SpriteSheetManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<SpriteSheet>& resource) override;
    void mt_Unload_Resource(SpriteSheet* resource) override;
};

class SpriteSheetAnimatorManager : public ResourceManager<SpriteSheetAnimator>
{
public:
    SpriteSheetAnimatorManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<SpriteSheetAnimator>& resource) override;
    void mt_Unload_Resource(SpriteSheetAnimator* resource) override;
};

#endif // _SPRITE_SHEET_MANAGER_HPP
