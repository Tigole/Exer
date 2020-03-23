#ifndef _SYSTEM_ANIMATION_HPP
#define _SYSTEM_ANIMATION_HPP 1

#include <list>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>
#include "../Animation.hpp"

namespace sys_anim
{

struct Animation
{
    std::vector<std::unique_ptr<IAnimation>> m_Animations;
    std::size_t m_Current_Animation;
    bool m_Persistant;
    bool m_Alive;
};

}

class SystemAnimation
{
public:
    void mt_Update(float elapsed_time);
    void mt_Draw(sf::RenderTarget& tgt);

    std::size_t mt_Create_List(bool persistant);
    void mt_Add_Animation(std::size_t list_index, std::vector<IAnimation*> a);
    bool mt_Is_Ended(std::size_t list_index);

private:
    //std::list<std::vector<std::unique_ptr<IAnimation>>> m_Animations;

    std::vector<sys_anim::Animation> m_Animations;
};


#endif // _SYSTEM_ANIMATION_HPP
