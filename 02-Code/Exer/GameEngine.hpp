#ifndef _GAME_ENGINE_HPP
#define _GAME_ENGINE_HPP 1

#include "IEngine.hpp"
#include "Context.hpp"

#include "Map.hpp"
#include "Dynamics.hpp"
#include "Command.hpp"
#include "Quest.hpp"
#include "Item.hpp"

#include "Systems/SystemScript.hpp"
#include "Systems/SystemDialog.hpp"
#include "Systems/SystemQuest.hpp"
#include "Systems/SystemFight.hpp"
#include "Systems/SystemInventory.hpp"

#include "GameStates.hpp"
#include "States/GameState_Fight.hpp"
#include "States/GameState_Game.hpp"
#include "States/GameState_CreateCharacter.hpp"

std::ostream& operator<<(std::ostream& o, GameStateType gs);


void fn_Main(void);


class GameEngine : public IEngine
{
public:
    void mt_Create(void);

    Resource<Map> m_Map;

public:
    Creature* mt_Get_Creature(const std::string& creature_id);
    Creature* m_Player;
    std::vector<Resource<Dynamic>> m_Dyn;

public:
    void mt_Play_Music(const std::string& music_id);
    std::string mt_Get_Music_Id(void);
    sf::Music* mt_Get_Music(void);
private:
    Resource<sf::Music> m_Music;

public:
    SystemScript m_Script;

public:
    void mt_Start_Fight(const std::map<int, std::vector<CommandFightCreature>>& fighters, std::function<void(void)> pfn_On_Victory, const std::string& music_id);
private:

public:
    void mt_Change_Map(const std::string& map_id, const sf::Vector2f& player_pos);

    void mt_New_Game(const std::string& file);


    SystemInventory m_Inventory;
    GameState_Item m_Inventory_State;

public:
    SystemQuest m_System_Quest;

public:
    GameState_CreateCharacter m_Creation_State;


protected:
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;
    void mt_On_Exit(void) override;

public:
    GameState_Game m_Game_State;
    GameState_Quest m_Quest_State;

    GameState_Fight m_Fight_State;
    GameState_Skill m_Skill_State;

public:
    void mt_Change_State(GameStateType new_state);
    State<GameStateType> m_State;

public:
    void mt_Set_Camera_Center(const sf::Vector2f& center);
    sf::View mt_Get_Camera_View(void) const;
private:
    sf::View m_Camera_View;

public:
    sf::Color m_Sky_Color;
private:
    float m_Sky_Color_Accumulated_Time_s;
    bool m_Sky_Command_Done;

public:
    void mt_End_Game(void);

    void mt_End_Demo(void);
};

#endif // _GAME_ENGINE_HPP
