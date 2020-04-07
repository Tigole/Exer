#include "GameState_Fight.hpp"
#include "../GameEngine.hpp"

/*void GameState_Fight::mt_On_Entry()
{
    //
}*/

void GameState_Fight::mt_Create()
{}

void GameState_Fight::mt_Destroy(void)
{}

void GameState_Fight::mt_Handle_Event(sf::Event& event)
{
    m_Fight.mt_Handle_Event(event);
}

void GameState_Fight::mt_Update(float delta_time_s)
{
    if (0 && !Context::smt_Get().m_Engine->m_Script.m_User_Ctrl)
    {
        Context::smt_Get().m_Engine->m_Script.mt_Process_Command(delta_time_s);
    }
    else
    {
        Context::smt_Get().m_Engine->m_Game_State.mt_Update(delta_time_s);

        m_Fight.mt_Update(delta_time_s);
        if (m_Fight.mt_Is_Ended())
        {
            Context::smt_Get().m_Engine->m_Script.mt_Complete_Current_Command();
            Context::smt_Get().m_Engine->m_Script.mt_Add_Command(new Command_Music("", 0.5f));
            Context::smt_Get().m_System_Animation = &Context::smt_Get().m_Engine->m_Animation;

            if (m_Fight.m_Player_Win == true)
            {
                Context::smt_Get().m_Engine->m_State = GameStateType::Game;
                Context::smt_Get().m_Engine->m_Script.mt_Add_Command(new Command_Music(m_Previous_Music_Id, 0.5f));
                Context::smt_Get().m_Engine->m_Script.mt_Add_Command(new Command_Camera_Creature(Context::smt_Get().m_Engine->m_Player, 2.0f, new Interpolator_Gain(2.0f)));
                if (m_Fight.m_pfn_On_Victory != nullptr)
                {
                    m_Fight.m_pfn_On_Victory();
                }
            }
            else
            {
                Context::smt_Get().m_Engine->m_State = GameStateType::Game_Over;
            }


            /*auto d = std::remove_if(Context::smt_Get().m_Engine->m_Dyn.begin(), Context::smt_Get().m_Engine->m_Dyn.end(), [](const Resource<Dynamic>& d)
                                    {
                                        const Creature* l_Creature = dynamic_cast<Creature*>(d.m_Resource);

                                        return ((l_Creature != nullptr) && (l_Creature->m_Gameplay_Data.m_Health <= 0));
                                    });
            if (d != Context::smt_Get().m_Engine->m_Dyn.end())
            {
                for (auto dd = d; dd != Context::smt_Get().m_Engine->m_Dyn.end(); dd++)
                {
                    Context::smt_Get().m_Dynamics.mt_Release(*dd);
                }
                Context::smt_Get().m_Engine->m_Dyn.erase(d, Context::smt_Get().m_Engine->m_Dyn.end());
            }*/
        }
    }
}

void GameState_Fight::mt_Draw(sf::RenderTarget& target)
{
    Context::smt_Get().m_Engine->m_Game_State.mt_Draw(target);
    target.setView(Context::smt_Get().m_Engine->m_Camera.mt_Get_View());
    m_Fight.mt_Draw(target);
    Context::smt_Get().m_Engine->m_Game_State.mt_Draw_Gameplay_Data(target);
}
