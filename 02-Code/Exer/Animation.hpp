#ifndef _ANIMATION_HPP
#define _ANIMATION_HPP 1

#include <SFML/Graphics.hpp>
#include "SpriteSheet.hpp"

class IAnimation
{
public:
    virtual void mt_Start(void) = 0;
    virtual void mt_Update(float elapsed_time) = 0;
    virtual void mt_Draw(sf::RenderTarget& tgt) = 0;

    bool m_Completed;
    bool m_Started;
};


class Animation_Skill : public IAnimation
{
public:
    Animation_Skill(const sf::Vector2f& position, const sf::Vector2f& dimension);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;
    void mt_Draw(sf::RenderTarget& tgt) override;

    Resource<SpriteSheet> m_SpriteSheet;
    float m_Accumulated_Time_s;
    std::size_t m_Current_Phase;
    std::vector<sf::Vector2<std::size_t>> m_Phases;
    sf::Vector2f m_Position;
    sf::Vector2f m_Dimension;
    sf::Sprite m_Sprite;
    sf::Transformable m_Transform;
};



struct Animation_Data
{
    std::vector<sf::Vector2<std::size_t>> m_Phases;
    Resource<SpriteSheet> m_SpriteSheet;
};

class Animation : public IAnimation
{
public:
    Animation(const sf::Vector2f& position, const sf::Vector2f& dimension, Resource<Animation_Data> data);
    ~Animation();

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;
    void mt_Draw(sf::RenderTarget& tgt) override;

    float m_Accumulated_Time_s;
    std::size_t m_Current_Phase;
    sf::Vector2f m_Position;
    sf::Vector2f m_Dimension;
    sf::Sprite m_Sprite;
    sf::Transformable m_Transform;
    Resource<Animation_Data> m_Data;
};


#endif // _ANIMATION_HPP
