#include "Event.hpp"





EventType fn_EventType(const sf::Event& e)
{
    EventType l_Ret(EventType::Ignore);

    if (e.type == sf::Event::KeyReleased)
    {
        if (e.key.code == sf::Keyboard::Up) l_Ret = EventType::Released_Up;
        else if (e.key.code == sf::Keyboard::Down) l_Ret = EventType::Released_Down;
        else if (e.key.code == sf::Keyboard::Left) l_Ret = EventType::Released_Left;
        else if (e.key.code == sf::Keyboard::Right) l_Ret = EventType::Released_Right;
        else if (e.key.code == sf::Keyboard::Space) l_Ret = EventType::Validate;
        else if (e.key.code == sf::Keyboard::Escape) l_Ret = EventType::Cancel;
    }
    else if (e.type == sf::Event::KeyPressed)
    {
        if (e.key.code == sf::Keyboard::Up) l_Ret = EventType::Pressed_Up;
        else if (e.key.code == sf::Keyboard::Down) l_Ret = EventType::Pressed_Down;
        else if (e.key.code == sf::Keyboard::Left) l_Ret = EventType::Pressed_Left;
        else if (e.key.code == sf::Keyboard::Right) l_Ret = EventType::Pressed_Right;
    }

    return l_Ret;
}

bool fn_EventType_Up(const sf::Event& e, bool pressed)
{
    return pressed ? e == EventType::Pressed_Up : e == EventType::Released_Up;
}

bool fn_EventType_Down(const sf::Event& e, bool pressed)
{
    return pressed ? e == EventType::Pressed_Down : e == EventType::Released_Down;
}

bool fn_EventType_Left(const sf::Event& e, bool pressed)
{
    return pressed ? e == EventType::Pressed_Left : e == EventType::Released_Left;
}

bool fn_EventType_Right(const sf::Event& e, bool pressed)
{
    return pressed ? e == EventType::Pressed_Right : e == EventType::Released_Right;
}

bool fn_EventType_Validate(const sf::Event& e)
{
    return e == EventType::Validate;
}

bool fn_EventType_Cancel(const sf::Event& e)
{
    return e == EventType::Cancel;
}


bool operator==(const sf::Event& e, const EventType& t)
{
    return fn_EventType(e) == t;
}

bool operator!=(const sf::Event& e, const EventType& t)
{
    return fn_EventType(e) != t;
}
