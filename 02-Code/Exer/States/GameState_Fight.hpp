#ifndef _GAME_STATE_FIGHT_HPP
#define _GAME_STATE_FIGHT_HPP 1

#include "../GameStates.hpp"



class GameState_Fight : public GameState
{
public:
    //void mt_On_Entry() override;
    void mt_Create(void) override;
    void mt_Destroy(void) override;
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;


    SystemFight m_Fight;
    std::string m_Previous_Music_Id;
};



#endif // _GAME_STATE_FIGHT_HPP
