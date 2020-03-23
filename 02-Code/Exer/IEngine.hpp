#ifndef _IENGINE_HPP
#define _IENGINE_HPP 1

#include <SFML/Graphics.hpp>

class IEngine
{
public:
    IEngine() : m_FPS(50) {}
    virtual ~IEngine(){}

    virtual void mt_Create(void) = 0;

    void mt_Run(void)
    {
        sf::Event l_event;
        sf::Clock l_clk;
        float l_FPS_sleep_time_ms(1000 / m_FPS);
        float l_sleep_time_ms;
        sf::Time l_elapsed_time;

        m_b_Close_Window = false;

        while (m_Wnd.isOpen())
        {
            l_elapsed_time = l_clk.restart();
            while(m_Wnd.pollEvent(l_event))
            {
                if (l_event.type == sf::Event::Closed)
                {
                    m_b_Close_Window = true;
                }

                mt_Handle_Event(l_event);
            }

            mt_Update(l_elapsed_time.asSeconds());

            m_Wnd.clear();
            mt_Draw(m_Wnd);
            m_Wnd.display();

            if (m_b_Close_Window == true)
            {
                m_Wnd.close();
            }
            else
            {
                l_sleep_time_ms = l_FPS_sleep_time_ms - l_clk.getElapsedTime().asMilliseconds();
                sf::sleep(sf::milliseconds(l_sleep_time_ms));
            }
        }

        mt_On_Exit();
    }

protected:
    sf::RenderWindow m_Wnd;
    bool m_b_Close_Window;
    int m_FPS;

    virtual void mt_Handle_Event(sf::Event& event) = 0;
    virtual void mt_Update(float delta_time_s) = 0;
    virtual void mt_Draw(sf::RenderTarget& target) = 0;
    virtual void mt_On_Exit(void) = 0;
};

#endif // _IENGINE_HPP
