#include "SystemScript.hpp"




SystemScript::SystemScript()
 :  m_User_Ctrl(true),
    m_Light_Ctrl(false),
    m_Check_Dyn_Collision(true)
{}


void SystemScript::mt_Add_Command(Command* c)
{
    m_Commands.push_back(std::unique_ptr<Command>(c));
    m_User_Ctrl = false;
    m_Check_Dyn_Collision = false;
    std::cout << "SCRIPT: Command count: " << m_Commands.size() << '\n';
}

void SystemScript::mt_Process_Command(float elapsed_time)
{
    m_User_Ctrl = m_Commands.empty();

    if (!m_Commands.empty())
    {
        if (m_Commands.front()->m_Completed == false)
        {
            if (m_Commands.front()->m_Started == false)
            {
                m_Commands.front()->mt_Start();
                m_Commands.front()->m_Started = true;
                m_Check_Dyn_Collision = m_Commands.front()->m_Check_Dyn_Collision;
            }
            m_Commands.front()->mt_Update(elapsed_time);
        }
        else
        {
            m_Commands.pop_front();
            m_Light_Ctrl = false;
            std::cout << "SCRIPT: Remaining command count: " << m_Commands.size() << "\n";
        }
    }
    else
    {
        m_Check_Dyn_Collision = true;
    }
}

void SystemScript::mt_Complete_Current_Command(void)
{
    if (!m_Commands.empty())
    {
        m_Commands.front()->m_Completed = true;
    }
}
