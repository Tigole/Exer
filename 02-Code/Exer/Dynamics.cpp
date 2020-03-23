#include "Dynamics.hpp"
#include "Context.hpp"
#include "GameEngine.hpp"

Dynamic::Dynamic(const std::string& name)
 :  m_Pos(0.0f, 0.0f),
    m_Vel(0.0f, 0.0f),
    m_Desired_Vel(0.0f, 0.0f),
    m_Friendly(true),
    m_Solid_Map(true),
    m_Solid_Dyn(true),
    m_Name(name),
    m_Dim(1.0f, 1.0f),
    m_Hitbox_Margin(0.2f)
{}

Dynamic::~Dynamic()
{
    //
}

sf::FloatRect Dynamic::mt_Get_Hit_Box(void) const
{
    sf::FloatRect l_Ret(m_Pos, m_Dim);

    l_Ret.top += m_Hitbox_Margin;
    l_Ret.left += m_Hitbox_Margin;
    l_Ret.width -= 2* m_Hitbox_Margin;
    l_Ret.height -= 2* m_Hitbox_Margin;

    return l_Ret;
}


Creature::Creature(const std::string& name, Resource<SpriteSheetAnimator> sprite)
 :  Dynamic(name),
    m_Gameplay_Data(),
    m_Sprite(sprite),
    m_Sprite_Data(),
    m_State(CreatureState::Idle)
{
    m_Gameplay_Data.m_Health = 10;
    m_Gameplay_Data.m_Health_Max = 10;
    m_Gameplay_Data.m_Data_Aventure.m_Psy = 5;
    m_Gameplay_Data.m_Data_Aventure.m_Psy_Max = 7;
    m_Sprite_Data.m_Switch_Time_s = 0.3f;

    m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::South;

    m_Pos = {0.0f, 0.0f};
    m_Dialog_Color = sf::Color::White;
}

Creature::Creature()
 :  Dynamic("")
{
    m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::South;
    m_Dialog_Color = sf::Color::White;
}

Creature::~Creature()
{
    if (m_Sprite != nullptr)
    {
        Context::smt_Get().m_SpriteSheetAnimators.mt_Release(m_Sprite);
    }
}

void Creature::mt_Draw(sf::RenderTarget& target, sf::RenderStates state)
{
    if (1 || 1/*camera.getViewport().intersects({m_Pos, {m_Sprite_Rect.width, m_Sprite_Rect.height}})*/)
    {
        sf::Sprite l_Sprite;
        sf::Vector2f l_Pos(m_Pos);

        l_Pos = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Pos);

        l_Sprite.setPosition(l_Pos);
        l_Sprite.setTexture(m_Sprite->mt_Get_Texture());
        l_Sprite.setTextureRect(m_Sprite_Rect);
        if (m_Gameplay_Data.m_Health <= 0)
        {
            l_Sprite.setColor(sf::Color(255, 255, 255, 64));
        }
        target.draw(l_Sprite, state);

        sf::RectangleShape l_ret;
        sf::FloatRect l_hit_box = mt_Get_Hit_Box();

        l_ret.setPosition(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(sf::Vector2f(l_hit_box.left, l_hit_box.top)));
        l_ret.setSize(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(sf::Vector2f(l_hit_box.width, l_hit_box.height)));
        l_ret.setFillColor(sf::Color::Transparent);
        l_ret.setOutlineColor(sf::Color::Black);
        l_ret.setOutlineThickness(2.0f);
        //target.draw(l_ret);
    }
}

void Creature::mt_Update(float elapsed_time)
{
    bool l_b_Moving((m_Vel.x != 0.0f) || (m_Vel.y != 0.0f));
    sf::Vector2f l_Dir(m_Vel/(sqrtf(m_Vel.x*m_Vel.x + m_Vel.y*m_Vel.y)));

    m_Sprite_Rect = m_Sprite->mt_Get_Rect(elapsed_time, l_Dir, l_b_Moving, m_Sprite_Data);

    if (m_Gameplay_Data.m_Health <= 0)
    {
        m_State = CreatureState::Dead;
        m_Solid_Dyn = false;
        m_Solid_Map = false;
    }
    else if (l_b_Moving)
    {
        m_State = CreatureState::Moving;
    }
    else
    {
        m_State = CreatureState::Idle;
    }
}

void Creature::mt_Set_Sprite(const std::string& sprite_id)
{
    if (m_Sprite != nullptr)
    {
        Context::smt_Get().m_SpriteSheetAnimators.mt_Release(m_Sprite);
    }
    m_Sprite = Context::smt_Get().m_SpriteSheetAnimators.mt_Get_Resource(sprite_id);
}

void Creature::mt_OnInteract(Creature* player)
{}

SpriteSheet_Direction Creature::mt_Get_Facing_Dir(void)
{
    return m_Sprite_Data.m_Facing_Dir;
}

void Creature::mt_LookAt(const Creature& tgt)
{
    mt_LookAt(tgt.m_Pos);
}

void Creature::mt_LookAt(const sf::Vector2f& cell)
{
    sf::Vector2f l_Direction(cell.x - m_Pos.x, cell.y - m_Pos.y);

    mt_LookDir(l_Direction);
}

void Creature::mt_LookDir(const sf::Vector2f& direction)
{
    if (fabs(direction.y) > fabs(direction.x))
    {
        if (direction.y < 0.0f)
        {
            m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::North;
        }
        else
        {
            m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::South;
        }
    }
    else
    {
        if (direction.x < 0.0f)
        {
            m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::West;
        }
        else
        {
            m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::East;
        }
    }
}



Teleport::Teleport(const sf::Vector2f& current_pos, const std::string& target_map_id, const sf::Vector2f& target_map_pos)
 :  Dynamic(""),
    m_Map_Id(target_map_id),
    m_Map_Pos(target_map_pos)
{
    m_Pos = current_pos;
    m_Solid_Dyn = false;
    m_Solid_Map = false;
}

void Teleport::mt_Draw(sf::RenderTarget& target, sf::RenderStates state)
{
    sf::RectangleShape l_rect;
    sf::Vector2f l_v;

    l_v = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_v);

    l_rect.setSize(l_v);
    l_rect.setPosition(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(m_Pos));

    l_rect.setFillColor(sf::Color::Transparent);
    l_rect.setOutlineColor(sf::Color::White);
    l_rect.setOutlineThickness(2.0f);

    target.draw(l_rect, state);
}

void Teleport::mt_Update(float elapsed_time)
{
    //
}

void Teleport::mt_OnInteract(Creature* player)
{}



void Dynamic_Script::mt_Draw(sf::RenderTarget& target, sf::RenderStates state)
{
    sf::RectangleShape l_Rect;

    l_Rect.setPosition(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(m_Pos));
    l_Rect.setSize(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(m_Dim));
    l_Rect.setFillColor(sf::Color::Transparent);
    l_Rect.setOutlineColor(sf::Color::Cyan);
    l_Rect.setOutlineThickness(2.0f);

    //target.draw(l_Rect);
}


void Dynamic_Script::mt_OnInteract(Creature* player)
{}



Dynamic_Chest::Dynamic_Chest() : Dynamic("")
{
    m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::South;
}

void Dynamic_Chest::mt_Draw(sf::RenderTarget& target, sf::RenderStates state)
{
    if (1 || 1/*camera.getViewport().intersects({m_Pos, {m_Sprite_Rect.width, m_Sprite_Rect.height}})*/)
    {
        sf::Sprite l_Sprite;
        sf::Vector2f l_Pos(m_Pos);

        l_Pos = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Pos);

        l_Sprite.setPosition(l_Pos);
        l_Sprite.setTexture(m_Sprite->mt_Get_Texture());
        l_Sprite.setTextureRect(m_Sprite_Rect);
        target.draw(l_Sprite, state);
    }
}

void Dynamic_Chest::mt_Update(float elapsed_time)
{
    m_Sprite_Rect = m_Sprite->mt_Get_Rect(0.0f, sf::Vector2f(0.0f, 0.0f), false, m_Sprite_Data);
}

void Dynamic_Chest::mt_OnInteract(Creature* player)
{
    if (m_Open == false)
    {
        std::vector<sf::String> l_Dialog;
        SystemScript& l_Script = Context::smt_Get().m_Engine->m_Script;

        l_Script.mt_Add_Command(new Command_ShowDialog({fn_Dialog(Context::smt_Get().m_Engine->m_Player, "Qu'y a-t-il dans ce coffre ?")}));

        for(std::size_t ii = 0; ii < m_Items.size(); ii++)
        {
            Item* l_Item;
            Context::smt_Get().m_Engine->m_Inventory.mt_Change_Item_Count(m_Items[ii].m_Item_Id, m_Items[ii].m_Quantity, m_Items[ii].m_Item_Type);

            l_Item = Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item<Item>(m_Items[ii].m_Item_Id, m_Items[ii].m_Item_Type);

            l_Dialog.push_back(sf::String(l_Item->m_Name + " (x " + std::to_string(m_Items[ii].m_Quantity) + ")"));
        }
        m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::North;

        l_Script.mt_Add_Command(new Command_ChestOpen(this));
        l_Script.mt_Add_Command(new Command_ShowDialog(l_Dialog));
        l_Script.mt_Add_Command(new Command_ShowDialog({fn_Dialog(Context::smt_Get().m_Engine->m_Player, "Intéressant...")}));

        m_Open = true;
    }
}







Dynamic_Light::Dynamic_Light() : Dynamic("")
{
    m_Solid_Dyn = false;
    m_Solid_Map = false;
    m_Start = false;
}

void Dynamic_Light::mt_Draw(sf::RenderTarget& target, sf::RenderStates state)
{
    sf::RectangleShape l_Rect;

    l_Rect.setFillColor(sf::Color::Transparent);
    l_Rect.setOutlineColor(sf::Color::Magenta);
    l_Rect.setOutlineThickness(2.0f);
    l_Rect.setPosition(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(m_Pos));
    l_Rect.setSize(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(m_Dim));

    //target.draw(l_Rect);
}

void Dynamic_Light::mt_Update(float elapsed_time)
{
    if (m_Start == true)
    {
        sf::Color l_Color;
        Interpolator_Linear l_Interpolator;
        m_Accumulated_Time_s += elapsed_time;

        l_Color = fn_Interpolate(&l_Interpolator, m_Accumulated_Time_s, m_Duration_s, m_Start_Color, m_Final_Color);

        Context::smt_Get().m_Engine->m_Sky_Color = l_Color;
        if (m_Accumulated_Time_s >= m_Duration_s)
        {
            m_Start = false;
        }
    }
}

void Dynamic_Light::mt_OnInteract(Creature* player)
{
    if (m_Start == false)
    {
        m_Start = true;
        m_Accumulated_Time_s = 0.0f;
        m_Start_Color = Context::smt_Get().m_Engine->m_Sky_Color;
    }
}


