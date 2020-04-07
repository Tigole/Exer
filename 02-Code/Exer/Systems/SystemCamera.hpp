#ifndef _SYSTEM_CAMERA_HPP
#define _SYSTEM_CAMERA_HPP 1

#include <SFML/Graphics.hpp>

class Creature;

enum class CameraMode
{
    Linked_To_Target,
    Linked_To_Position,
};


class SystemCamera
{
public:
    void mt_Initialize(const sf::RenderWindow& wnd);

    void mt_Set_Position(const sf::Vector2f& pos_pix);
    void mt_Set_Tgt(const Creature* tgt);
    void mt_Back_To_Tgt(void);
    const Creature* mt_Get_Tgt(void) const;

    void mt_Update(float elapsed_time);

    CameraMode mt_Get_Mode(void) const;

    sf::View mt_Get_View(void) const;

private:
    const Creature* m_Tgt;
    sf::Vector2f m_Pos;
    CameraMode m_Mode;
    sf::View m_View;
};

#endif // _SYSTEM_CAMERA_HPP
