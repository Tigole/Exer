#include "SystemScript.hpp"




SystemScript::SystemScript()
 :  m_User_Ctrl(true)
{}


void SystemScript::mt_Add_Command(Command* c)
{
    m_Commands.push_back(std::unique_ptr<Command>(c));
    m_User_Ctrl = false;
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
                m_Camera_Ctrl = false;
                m_Commands.front()->mt_Start();
                m_Commands.front()->m_Started = true;
            }
            m_Commands.front()->mt_Update(elapsed_time);
        }
        else
        {
            m_Commands.pop_front();
            m_Light_Ctrl = false;
        }
    }
    else
    {
        m_Camera_Ctrl = true;
    }
}

void SystemScript::mt_Complete_Current_Command(void)
{
    if (!m_Commands.empty())
    {
        m_Commands.front()->m_Completed = true;
    }
}


