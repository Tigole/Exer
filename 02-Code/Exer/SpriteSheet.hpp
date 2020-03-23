#ifndef _SPRITE_SHEET_HPP
#define _SPRITE_SHEET_HPP 1

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include "Managers/ResourceManager.hpp"

class SpriteSheet
{
public:
    SpriteSheet();

    bool mt_Load_From_File(const std::string& ref_file);
    sf::IntRect mt_Get_Rect(std::size_t xx, std::size_t yy);
    sf::Texture& mt_Get_Texture(void);

private:
    std::size_t m_XX;
    std::size_t m_YY;
    std::string m_Texture_Id;
    Resource<sf::Texture> m_Texture;
};

enum class SpriteSheet_Direction
{
    South,
    West,
    East,
    North,
};

struct SpriteSheetAnimatorData
{
    SpriteSheetAnimatorData() : m_Switch_Time_s(0.5f), m_Old_Offset(0, 0) {}
    float m_Switch_Time_s;
    sf::Vector2i m_Old_Offset;
    SpriteSheet_Direction m_Facing_Dir;
};

class SpriteSheetAnimator
{
public:
    bool mt_Load_From_File(const std::string& ref_file);
    sf::IntRect mt_Get_Rect(float elapsed_time_s, const sf::Vector2f& orientation, bool moving, SpriteSheetAnimatorData& data);
    sf::Texture& mt_Get_Texture(void){return m_Sheet->mt_Get_Texture();}
    sf::FloatRect mt_Get_HitBox_Margins(const SpriteSheetAnimatorData& data);

private:
    sf::Vector2i m_Offset;
    Resource<SpriteSheet> m_Sheet;
    float m_Switch_Time_Ref_s;
    std::map<sf::Vector2i, sf::FloatRect> m_Margins;
};

#endif // _SPRITE_SHEET_HPP
