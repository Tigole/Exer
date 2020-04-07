#include "Animation.hpp"
#include "Context.hpp"

#include <iostream>

Animation_Skill::Animation_Skill(const sf::Vector2f& position, const sf::Vector2f& dimension)
 :  m_Position(position),
    m_Dimension(dimension)
{}

void Animation_Skill::mt_Start(void)
{
    m_SpriteSheet = Context::smt_Get().m_SpriteSheets.mt_Get_Resource("Sword4");

    m_Sprite.setTexture(m_SpriteSheet->mt_Get_Texture());
    m_Sprite.setTextureRect(m_SpriteSheet->mt_Get_Rect(0, 0));

    m_Phases.push_back(sf::Vector2<std::size_t>(0, 0));
    m_Phases.push_back(sf::Vector2<std::size_t>(1, 0));
    m_Phases.push_back(sf::Vector2<std::size_t>(2, 0));
    m_Phases.push_back(sf::Vector2<std::size_t>(3, 0));
    m_Phases.push_back(sf::Vector2<std::size_t>(4, 0));
    m_Phases.push_back(sf::Vector2<std::size_t>(0, 1));
    m_Phases.push_back(sf::Vector2<std::size_t>(1, 1));
    m_Phases.push_back(sf::Vector2<std::size_t>(2, 1));
    m_Phases.push_back(sf::Vector2<std::size_t>(3, 1));
    m_Phases.push_back(sf::Vector2<std::size_t>(4, 1));
    m_Phases.push_back(sf::Vector2<std::size_t>(0, 2));
    m_Phases.push_back(sf::Vector2<std::size_t>(1, 2));
    m_Phases.push_back(sf::Vector2<std::size_t>(2, 2));
    m_Phases.push_back(sf::Vector2<std::size_t>(3, 2));
    m_Phases.push_back(sf::Vector2<std::size_t>(4, 2));

    m_Current_Phase = 0;

    m_Transform.setPosition(m_Position);
    m_Transform.setScale(m_Dimension.x / m_Sprite.getTextureRect().width, m_Dimension.y / m_Sprite.getTextureRect().height);
    m_Transform.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);
}

void Animation_Skill::mt_Update(float elapsed_time)
{
    m_Accumulated_Time_s += elapsed_time;
    if (m_Accumulated_Time_s >= 0.1f)
    {
        m_Accumulated_Time_s = 0.0f;
        m_Current_Phase++;
        if (m_Current_Phase < m_Phases.size())
        {
            m_Sprite.setTextureRect(m_SpriteSheet->mt_Get_Rect(m_Phases[m_Current_Phase].x, m_Phases[m_Current_Phase].y));
        }
        else
        {
            m_Completed = true;
        }
    }
}

void Animation_Skill::mt_Draw(sf::RenderTarget& tgt)
{
    tgt.draw(m_Sprite, m_Transform.getTransform());
}











Animation::Animation(const sf::Vector2f& position, const sf::Vector2f& dimension, Resource<Animation_Data> data)
 :  m_Position(position),
    m_Dimension(dimension),
    m_Data(data)
{}

Animation::~Animation()
{
    Context::smt_Get().m_Animations.mt_Release(m_Data);
}

void Animation::mt_Start(void)
{
    m_Sprite.setTexture(m_Data->m_SpriteSheet->mt_Get_Texture());
    m_Sprite.setTextureRect(m_Data->m_SpriteSheet->mt_Get_Rect(0, 0));

    m_Current_Phase = 0;
    m_Current_Sound = 0;

    m_Transform.setPosition(m_Position);
    m_Transform.setScale(m_Dimension.x / m_Sprite.getTextureRect().width, m_Dimension.y / m_Sprite.getTextureRect().height);
    m_Transform.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);

    std::cout << "Animation frames: " << m_Data->m_Phases.size() << '\n';
    m_Accumulated_Time_s = 0.0f;
}

void Animation::mt_Update(float elapsed_time)
{
    m_Accumulated_Time_s += elapsed_time;

    if (m_Current_Sound < m_Data->m_Sounds.size())
    {
        if (m_Data->m_Sounds[m_Current_Sound].first == m_Current_Phase)
        {
            Context::smt_Get().m_System_Sound.mt_Play_Sound(m_Data->m_Sounds[m_Current_Sound].second, sf::Vector3f(0.0f, 0.0f, 0.0f), true);
            m_Current_Sound++;
        }
    }

    if (m_Accumulated_Time_s >= 0.1f)
    {
        m_Accumulated_Time_s = 0.0f;
        m_Current_Phase++;
        if (m_Current_Phase < m_Data->m_Phases.size())
        {
            m_Sprite.setTextureRect(m_Data->m_SpriteSheet->mt_Get_Rect(m_Data->m_Phases[m_Current_Phase].x, m_Data->m_Phases[m_Current_Phase].y));
        }
        else
        {
            m_Completed = true;
        }
    }
}

void Animation::mt_Draw(sf::RenderTarget& tgt)
{
    tgt.draw(m_Sprite, m_Transform.getTransform());
}


