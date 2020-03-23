#ifndef _GAME_STATE_GAME_HPP
#define _GAME_STATE_GAME_HPP 1

#include "../GameStates.hpp"

class GameState_Game : public GameState
{
public:
    //void mt_On_Entry() override;
    void mt_Create(void) override;
    void mt_Destroy(void) override;
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float delta_time_s) override;
    void mt_Draw(sf::RenderTarget& target) override;

    void mt_Draw_HUD(sf::RenderTarget& target) const;
    void mt_Draw_Gameplay_Data(sf::RenderTarget& target) const;

    SystemDialog m_Dialog;
    Resource<sf::Shader> m_Shader;
};


#endif // _GAME_STATE_GAME_HPP
