#ifndef _GAME_STATE_CHOOSE_CLASS_HPP
#define _GAME_STATE_CHOOSE_CLASS_HPP 1

#include "../GameStates.hpp"

struct ClassData
{
    Resource<SpriteSheetAnimator> m_Sprite;
    SpriteSheetAnimatorData m_Sprite_Data;
    sf::String m_Class_Name;
    sf::String m_Class_Description;
    std::string m_Class_Skill;
    sf::IntRect m_Sprite_Rect;
    GameplayData m_GameplayData;
    sf::Color m_Text_Color;
};

class GameState_CreateCharacter : public GameState
{
public:
    void mt_Create(void) override;
    void mt_Destroy(void) override;
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;

private:
    std::vector<ClassData> m_Classes;
    std::size_t m_Selected_Class;

    Resource<sf::Font> m_Font;

    sf::String m_Player_Name;
    bool m_Name_Set;
};

#endif // _GAME_STATE_CHOOSE_CLASS_HPP
