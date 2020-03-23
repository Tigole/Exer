#include "SystemDialog.hpp"
#include "../Context.hpp"
#include "../GameEngine.hpp"

#include "RichText.hpp"

SystemDialog::SystemDialog()
 :  m_Dialog(),
    m_Choices(),
    m_Current_Line(0),
    m_Show_Dialog(false),
    m_Show_Choices(false),
    m_Dialog_Pos(),
    m_Dialog_Font()
{}


void SystemDialog::mt_Handle_Event(sf::Event& event)
{
    if (m_Show_Choices || m_Show_Dialog)
    {
        if (m_Show_Dialog)
        {
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    m_Current_Line++;
                    if (m_Current_Line >= m_Dialog.size())
                    {
                        m_Show_Dialog = false;
                        Context::smt_Get().m_Engine->m_Script.mt_Complete_Current_Command();
                    }
                    Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
                }
            }
        }
        else
        {
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                {
                    m_Show_Choices = false;
                    m_Choices[m_Current_Line].m_Callback();
                    Context::smt_Get().m_Engine->m_Script.mt_Complete_Current_Command();
                    Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
                }
                else if ((event.key.code == sf::Keyboard::Up) || (event.key.code == sf::Keyboard::Left))
                {
                    if (m_Current_Line > 0)
                    {
                        m_Current_Line--;
                        Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
                    }
                }
                else if ((event.key.code == sf::Keyboard::Down) || (event.key.code == sf::Keyboard::Right))
                {
                    if (m_Current_Line < (m_Choices.size()-1))
                    {
                        m_Current_Line++;
                        Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
                    }
                }
            }
        }
    }
}

void SystemDialog::mt_Update(float delta_time_s)
{
    if (m_Show_Dialog == true)
    {
        if (m_Current_Line >= m_Dialog.size())
        {
            m_Show_Dialog = false;
        }
    }
    /*else if (m_Show_Choices == true)
    {
        if ()
        {
            m_Show_Choices = false;
        }
    }*/
}

void SystemDialog::mt_Draw(sf::RenderTarget& target)
{
    if (m_Show_Dialog == true)
    {
        mt_Display_Dialog(target, m_Dialog[m_Current_Line]);
    }
    if (m_Show_Choices == true)
    {
        mt_Display_Choices(target);
    }
}


void SystemDialog::mt_Show_Dialog(const std::vector<sf::String>& lines)
{
    if (lines.size() != 0)
    {
        m_Dialog = lines;
        m_Show_Dialog = true;
        m_Current_Line = 0;
    }
}

void SystemDialog::mt_Show_Choice(const std::vector<DialogChoice>& choices)
{
    if (choices.size() != 0)
    {
        m_Choices = choices;
        m_Show_Choices = true;
        m_Current_Line = 0;
    }
}




void SystemDialog::mt_Display_Dialog(sf::RenderTarget& target, const sf::String& text)
{
    sfe::RichText l_Text;
    sf::RectangleShape l_Rect;
    float l_Margin(20.0f);

    l_Text.setFont(*m_Dialog_Font);
    l_Text.setString(text);
    l_Text.setPosition(target.getView().getSize().x/2 - l_Text.getGlobalBounds().width/2, target.getSize().y - l_Text.getGlobalBounds().height - l_Margin);
//    l_Text.setFillColor(sf::Color(255, 230, 230));

    l_Rect.setFillColor(sf::Color(20, 20, 150));
    l_Rect.setOutlineColor(sf::Color::White);
    l_Rect.setOutlineThickness(2.0f);
    l_Rect.setSize({l_Text.getGlobalBounds().width + 2*l_Margin, l_Text.getGlobalBounds().height + 2*l_Margin});
    l_Rect.setPosition(target.getView().getSize().x/2 - l_Rect.getGlobalBounds().width/2, target.getView().getSize().y - l_Rect.getGlobalBounds().height);

    target.draw(l_Rect);
    target.draw(l_Text);
}

void SystemDialog::mt_Display_Choices(sf::RenderTarget& target)
{
    std::vector<sfe::RichText> l_Text;
    sf::CircleShape l_Cursor;
    sf::RectangleShape l_Rect;
    float l_Margin(20.0f);
    sf::Vector2f l_Rect_Size(0.0f, 0.0f);
    bool l_Center_X = false;
    float l_Max_Size_y = 0.0f;

    l_Cursor.setPointCount(5);
    l_Cursor.setFillColor(sf::Color(218, 165, 32));
    l_Cursor.setRadius(5.0f);

    l_Text.resize(m_Choices.size());
    for (std::size_t ii = 0; ii < m_Choices.size(); ii++)
    {
        l_Text[ii].setFont(*m_Dialog_Font);
        //l_Text[ii].setFillColor(sf::Color(255, 230, 230));
        l_Text[ii].setString(m_Choices[ii].m_Text);

        l_Rect_Size.x = std::max(l_Rect_Size.x, l_Text[ii].getLocalBounds().width);
        l_Rect_Size.y += l_Text[ii].getLocalBounds().height;

        l_Max_Size_y = std::max(l_Max_Size_y, l_Text[ii].getGlobalBounds().height);
    }

    for (std::size_t ii = 0; ii < l_Text.size(); ii++)
    {
        float l_Pos_x;
        float l_Pos_y;

        if (l_Center_X)
        {
            l_Pos_x = target.getView().getSize().x/2 - l_Text[ii].getLocalBounds().width/2;
        }
        else
        {
            l_Pos_x = target.getView().getSize().x/2 - l_Rect_Size.x/2;
        }

        l_Pos_y = target.getView().getSize().y - (l_Text.size() - ii) * l_Text[ii].getLocalBounds().height - l_Margin;
        l_Pos_y = target.getView().getSize().y - (l_Text.size() - ii) * l_Max_Size_y - l_Margin;

        l_Text[ii].setPosition(l_Pos_x, l_Pos_y);

        if (ii == m_Current_Line)
            l_Cursor.setPosition(l_Text[ii].getPosition().x - 15.0f,
                                 l_Text[ii].getPosition().y + l_Text[ii].getGlobalBounds().height/2);
    }

    l_Rect_Size.x += l_Margin * 2.0f;
    l_Rect_Size.y += l_Margin * 2.0f;

    l_Rect.setFillColor(sf::Color(20, 20, 150));
    l_Rect.setOutlineColor(sf::Color::White);
    l_Rect.setOutlineThickness(2.0f);
    l_Rect.setSize(l_Rect_Size);
    l_Rect.setPosition(target.getView().getSize().x/2 - l_Rect.getLocalBounds().width/2, target.getView().getSize().y - l_Rect.getLocalBounds().height);

    target.draw(l_Rect);
    for (std::size_t ii = 0; ii < l_Text.size(); ii++)
        target.draw(l_Text[ii]);

    target.draw(l_Cursor);
}
