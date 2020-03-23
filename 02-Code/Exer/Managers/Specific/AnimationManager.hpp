#ifndef _ANIMATION_MANAGER_HPP
#define _ANIMATION_MANAGER_HPP 1

#include "../ResourceManager.hpp"
#include "../../Animation.hpp"

class AnimationManager : public ResourceManager<Animation_Data>
{
public:
    AnimationManager();

protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Animation_Data>& resource) override;
    void mt_Unload_Resource(Animation_Data* resource) override;
};

#endif // _ANIMATION_MANAGER_HPP
