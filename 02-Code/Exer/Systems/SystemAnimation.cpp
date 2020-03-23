#include "SystemAnimation.hpp"

void SystemAnimation::mt_Update(float elapsed_time)
{
    for (std::size_t ii = 0; ii < m_Animations.size(); ii++)
    {
        if (m_Animations[ii].m_Alive)
        {
            if (m_Animations[ii].m_Animations[m_Animations[ii].m_Current_Animation]->m_Completed == false)
            {
                if (m_Animations[ii].m_Animations[m_Animations[ii].m_Current_Animation]->m_Started == false)
                {
                    m_Animations[ii].m_Animations[m_Animations[ii].m_Current_Animation]->mt_Start();
                    m_Animations[ii].m_Animations[m_Animations[ii].m_Current_Animation]->m_Started = true;
                }
                m_Animations[ii].m_Animations[m_Animations[ii].m_Current_Animation]->mt_Update(elapsed_time);
            }
            else
            {
                m_Animations[ii].m_Current_Animation++;
            }

            if (m_Animations[ii].m_Current_Animation >= m_Animations[ii].m_Animations.size())
            {
                if (m_Animations[ii].m_Persistant == true)
                {
                    for (std::size_t jj = 0; jj < m_Animations[ii].m_Animations.size(); jj++)
                    {
                        m_Animations[ii].m_Animations[jj]->m_Completed = false;
                        m_Animations[ii].m_Animations[jj]->m_Started = false;
                    }
                    m_Animations[ii].m_Current_Animation = 0;
                }
                else
                {
                    m_Animations[ii].m_Alive = false;
                }
            }
        }
    }
}

void SystemAnimation::mt_Draw(sf::RenderTarget& tgt)
{
    for (std::size_t ii = 0; ii < m_Animations.size(); ii++)
    {
        if (m_Animations[ii].m_Alive)
        {
            m_Animations[ii].m_Animations[m_Animations[ii].m_Current_Animation]->mt_Draw(tgt);
        }
    }
}

std::size_t SystemAnimation::mt_Create_List(bool persistant)
{
    std::size_t l_Ret(-1);

    for (std::size_t ii = 0; (ii < m_Animations.size()) && (l_Ret == -1); ii++)
    {
        if (m_Animations[ii].m_Alive == false)
        {
            l_Ret = ii;
        }
    }

    if (l_Ret == -1)
    {
        m_Animations.push_back(sys_anim::Animation());
        l_Ret = m_Animations.size() -1;
    }

    m_Animations[l_Ret].m_Alive = false;
    m_Animations[l_Ret].m_Persistant = persistant;
    m_Animations[l_Ret].m_Current_Animation = 0;
    m_Animations[l_Ret].m_Animations.resize(0);

    return l_Ret;
}

void SystemAnimation::mt_Add_Animation(std::size_t list_index, std::vector<IAnimation*> a)
{
    for (std::size_t ii = 0; ii < a.size(); ii++)
    {
        m_Animations[list_index].m_Animations.push_back(std::unique_ptr<IAnimation>(a[ii]));
    }

    m_Animations[list_index].m_Alive = m_Animations[list_index].m_Current_Animation < m_Animations[list_index].m_Animations.size();
}

bool SystemAnimation::mt_Is_Ended(std::size_t list_index)
{
    return (m_Animations[list_index].m_Alive == false);
}

/*void SystemAnimation::mt_Update(float elapsed_time)
{
    m_User_Ctrl = m_Animations.empty();

    if (!m_Animations.empty())
    {
        for (std::size_t ii = 0; ii < m_Animations.front().size(); ii++)
        {
            if (m_Animations.front()[ii]->m_Completed == false)
            {
                if (m_Animations.front()[ii]->m_Started == false)
                {
                    m_Animations.front()[ii]->mt_Start();
                    m_Animations.front()[ii]->m_Started = true;
                }
                m_Animations.front()[ii]->mt_Update(elapsed_time);
            }
            else
            {
                m_Animations.pop_front();
            }
        }
    }
}

void SystemAnimation::mt_Draw(sf::RenderTarget& tgt)
{
    if (!m_Animations.empty())
    {
        for (std::size_t ii = 0; ii < m_Animations.front().size(); ii++)
        {
            m_Animations.front()[ii]->mt_Draw(tgt);
        }
    }
}

void SystemAnimation::mt_Add_Animation(std::vector<IAnimation*> a)
{
    std::vector<std::unique_ptr<IAnimation>> l_Animations;
    for (std::size_t ii = 0; ii < a.size(); ii++)
    {
        l_Animations.push_back(std::unique_ptr<IAnimation>(a[ii]));
    }
    m_Animations.push_back(std::move(l_Animations));
}*/

