#ifndef _GAME_STATES_HPP
#define _GAME_STATES_HPP 1

#include <SFML/Graphics.hpp>
#include "Context.hpp"
#include "Systems/SystemDialog.hpp"
#include "Systems/SystemFight.hpp"

enum class GameStateType
{
    Game,
    Display_Quests,
    Display_Inventory,
    Fight,
    Display_Skills,

    Choose_Class,

    Game_Over,

    EndGame,
    Close,

    COUNT
};


class GameState
{
public:
    virtual ~GameState(){}
    //virtual void mt_On_Entry() = 0;
    virtual void mt_Create(void) = 0;
    virtual void mt_Destroy(void) = 0;
    virtual void mt_Handle_Event(sf::Event& event) = 0;
    virtual void mt_Update(float delta_time_s) = 0;
    virtual void mt_Draw(sf::RenderTarget& target) = 0;
};

class GameStateDisplayer : public GameState
{
public:
    GameStateDisplayer();
    virtual ~GameStateDisplayer(){};

    void mt_Update(float delta_time_s) override;

    float m_Margin = 10.0f;
    float m_Accumulated_Time_s = 0.0f;
    float m_Gradient_Period_s = 2.0f;
    Interpolator_Cos m_Interpolator;
    sf::Color m_Inactive_Rect_Filling_Color = sf::Color::Transparent;
    sf::Color m_Inactive_Text_Color = sf::Color::White;
    sf::Color m_Selected_Rect_Filling_Color = sf::Color(255, 69, 0);
    sf::Color m_Selected_Color;

    sf::Color m_Color_Theme[2];
};

class GameState_Quest : public GameStateDisplayer
{
public:
    //void mt_On_Entry() override;
    void mt_Create(void) override;
    void mt_Destroy(void) override;
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;

    int m_Selected_Quest;
    Resource<sf::Font> m_Font;
};

class GameState_Item : public GameStateDisplayer
{
public:
    //void mt_On_Entry() override;
    void mt_Create(void) override;
    void mt_Destroy(void) override;
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;

    void mt_Lock_Type(ItemType item_type = ItemType::COUNT);

    Item* mt_Get_Selected_Item(void);

    std::map<ItemType, int> m_Current_Item;
    ItemType m_Item_Type;

    bool m_Select_Item_Type;

    bool m_Type_Locked;
};

class GameState_Skill : public GameStateDisplayer
{
public:
    //void mt_On_Entry() override;
    void mt_Create(void) override;
    void mt_Destroy(void) override;
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;

    ISkill* mt_Get_Selected_Skill(void);

    int m_Selected_Skill;
    Resource<sf::Font> m_Font;
    GameStateType m_Previous_State;
    sf::Vector2f m_Size = {3.0f, 4.0f};
};

class GameState_EndGame : public GameState
{
public:
    void mt_Create(void) override;
    void mt_Destroy(void) override;
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;
};


#endif // _GAME_STATES_HPP
