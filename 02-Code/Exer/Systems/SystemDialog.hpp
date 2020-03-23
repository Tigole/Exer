#ifndef _SYSTEM_DIALOG_HPP
#define _SYSTEM_DIALOG_HPP 1

#include <vector>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>
#include "../Managers/ResourceManager.hpp"

#include <functional>

struct DialogChoice
{
    DialogChoice() : m_Text(), m_Callback(nullptr) {}
    DialogChoice(const sf::String& text, std::function<void(void)> callback) : m_Text(text), m_Callback(callback) {}
    template<class C>
    DialogChoice(const sf::String& text, void(C::*callback)(void), C* object) :  m_Text(text)
    {
        m_Callback = std::bind(callback, object);
    }
    DialogChoice(const DialogChoice& rhs) : m_Text(rhs.m_Text), m_Callback(rhs.m_Callback) {}
    sf::String m_Text;
    std::function<void(void)> m_Callback;
};

class SystemDialog
{
public:
    SystemDialog();

    bool mt_Has_Event_Priority(void) const {return m_Show_Dialog || m_Show_Choices;}
    void mt_Handle_Event(sf::Event& event);
    void mt_Update(float delta_time_s);
    void mt_Draw(sf::RenderTarget& target);

    void mt_Show_Dialog(const std::vector<sf::String>& lines);
    void mt_Show_Choice(const std::vector<DialogChoice>& choices);

protected:
    void mt_Display_Dialog(sf::RenderTarget& target, const sf::String& text);
    void mt_Display_Choices(sf::RenderTarget& target);

public:
    std::vector<sf::String> m_Dialog;
    std::vector<DialogChoice> m_Choices;
    std::size_t m_Current_Line;
    bool m_Show_Dialog;
    bool m_Show_Choices;
    sf::Vector2f m_Dialog_Pos;
    Resource<sf::Font> m_Dialog_Font;
};

#endif // _SYSTEM_DIALOG_HPP
