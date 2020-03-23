#ifndef _SYSTEM_HPP
#define _SYSTEM_HPP 1

class System
{
public:
    virtual ~System(){}

    virtual void mt_Handle_Event(sf::Event& event) = 0;
    virtual void mt_Update(float delta_time_s) = 0;
    virtual void mt_Draw(sf::RenderTarget& target) = 0;
};

#endif // _SYSTEM_HPP
