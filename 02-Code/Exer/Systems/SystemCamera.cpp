#include "SystemCamera.hpp"
#include "../Dynamics.hpp"
#include "../Context.hpp"
#include "../GameEngine.hpp"


void SystemCamera::mt_Initialize(const sf::RenderWindow& wnd)
{
    m_View = wnd.getView();
}

void SystemCamera::mt_Set_Position(const sf::Vector2f& pos_pix)
{
    m_Mode = CameraMode::Linked_To_Position;
    m_Pos = pos_pix;
}

void SystemCamera::mt_Set_Tgt(const Creature* tgt)
{
    m_Mode = CameraMode::Linked_To_Target;
    m_Tgt = tgt;
}

void SystemCamera::mt_Back_To_Tgt(void)
{
    m_Mode = CameraMode::Linked_To_Target;
}

const Creature* SystemCamera::mt_Get_Tgt(void) const
{
    return m_Tgt;
}

void SystemCamera::mt_Update(float elapsed_time)
{
    if (m_Mode == CameraMode::Linked_To_Position)
    {
        m_View.setCenter(m_Pos);
    }
    else if (m_Mode == CameraMode::Linked_To_Target)
    {
        m_View.setCenter(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(m_Tgt->m_Pos/* + sf::Vector2f(0.5f, 0.5f)*/));
    }
}

CameraMode SystemCamera::mt_Get_Mode(void) const
{
    return m_Mode;
}

sf::View SystemCamera::mt_Get_View(void) const
{
    return m_View;
}
