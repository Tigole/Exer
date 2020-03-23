#ifndef _EVENT_HPP
#define _EVENT_HPP 1

#include <SFML/Window/Event.hpp>

enum class EventType
{
    Ignore,
    Pressed_Up,
    Pressed_Down,
    Pressed_Left,
    Pressed_Right,
    Released_Up,
    Released_Down,
    Released_Left,
    Released_Right,
    Validate,
    Cancel,

    COUNT,
};

EventType fn_EventType(const sf::Event& e);
bool fn_EventType_Up(const sf::Event& e, bool pressed);
bool fn_EventType_Down(const sf::Event& e, bool pressed);
bool fn_EventType_Left(const sf::Event& e, bool pressed);
bool fn_EventType_Right(const sf::Event& e, bool pressed);
bool fn_EventType_Validate(const sf::Event& e);
bool fn_EventType_Cancel(const sf::Event& e);

bool operator==(const sf::Event& e, const EventType& t);
bool operator!=(const sf::Event& e, const EventType& t);

#endif // _EVENT_HPP
