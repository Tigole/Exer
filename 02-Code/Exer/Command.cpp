#include "Command.hpp"
#include "Context.hpp"
#include "GameEngine.hpp"
#include "Interpolation.hpp"

Command::Command()
 :  m_Started(false),
    m_Completed(false)
{}

Command::~Command(){}




Command_MoveOffset::Command_MoveOffset(Creature* tgt, const std::vector<sf::Vector2i>& offsets, float speed)
 :  m_Tgt(tgt),
    m_Offsets(offsets),
    m_Obj_Speed(speed)
{}

void Command_MoveOffset::mt_Start(void)
{
    m_Current_Offset = 0;
    m_Old_Pos = m_Tgt->m_Pos;
    m_X_Done = false;
    m_Y_Done = false;

    m_X_Collision = false;
    m_Y_Collision = false;

    m_Obj_Init_Speed = m_Tgt->m_Speed;
    m_Tgt->m_Speed = m_Obj_Speed;
    m_Tgt->m_Desired_Vel = mt_Compute_Vel(sf::Vector2f(m_Offsets[m_Current_Offset].x, m_Offsets[m_Current_Offset].y), m_Tgt->m_Speed);
    Context::smt_Get().m_Engine->m_Script.m_Camera_Ctrl = true;
}

void Command_MoveOffset::mt_Update(float elapsed_time)
{
    if (m_Tgt->m_Desired_Vel.x != 0.0f)
    {
        if (m_Tgt->m_Vel.x == 0.0f)
        {
            //
        }
        if (m_Offsets[m_Current_Offset].x > 0.0f)
        {
            if ((m_Old_Pos.x + m_Offsets[m_Current_Offset].x) < m_Tgt->m_Pos.x)
            {
                m_X_Done = true;
            }
        }
        else if (m_Offsets[m_Current_Offset].x < 0.0f)
        {
            if ((m_Old_Pos.x + m_Offsets[m_Current_Offset].x) > m_Tgt->m_Pos.x)
            {
                m_X_Done = true;
            }
        }
    }
    else
    {
        m_X_Done = true;
    }

    if (m_X_Done == true)
    {
        m_Tgt->m_Desired_Vel.x = 0.0f;
        m_Tgt->m_Pos.x = (int)(m_Old_Pos.x + m_Offsets[m_Current_Offset].x);
    }

    if (m_Tgt->m_Desired_Vel.y != 0.0f)
    {
        if (m_Offsets[m_Current_Offset].y > 0.0f)
        {
            if ((m_Old_Pos.y + m_Offsets[m_Current_Offset].y) < m_Tgt->m_Pos.y)
            {
                m_Y_Done = true;
            }
        }
        else if (m_Offsets[m_Current_Offset].y < 0.0f)
        {
            if ((m_Old_Pos.y + m_Offsets[m_Current_Offset].y) > m_Tgt->m_Pos.y)
            {
                m_Y_Done = true;
            }
        }
    }
    else
    {
        m_Y_Done = true;
    }
    if (m_Y_Done == true)
    {
        m_Tgt->m_Pos.y = (int)(m_Old_Pos.y + m_Offsets[m_Current_Offset].y);
        m_Tgt->m_Desired_Vel.y = 0.0f;
    }

    //std::cout << m_X_Done << '\t' << m_Y_Done << '\t';
    //std::cout << m_Old_Pos.x + m_Offsets[m_Current_Offset].x - m_Tgt->m_Pos.x << '\t' << m_Old_Pos.y + m_Offsets[m_Current_Offset].y - m_Tgt->m_Pos.y << '\t';
    if ((m_X_Done == true) && (m_Y_Done == true))
    {
        m_Current_Offset++;
        m_Old_Pos = m_Tgt->m_Pos;
        m_X_Done = false;
        m_Y_Done = false;
    }

    if (m_Current_Offset >= m_Offsets.size())
    {
        m_Tgt->m_Speed = m_Obj_Init_Speed;
        m_Completed = true;
    }
    else
    {
        sf::Vector2f l_Diff;

        l_Diff.x = m_Old_Pos.x + m_Offsets[m_Current_Offset].x - m_Tgt->m_Pos.x;
        l_Diff.y = m_Old_Pos.y + m_Offsets[m_Current_Offset].y - m_Tgt->m_Pos.y;

        //m_Tgt->m_Desired_Vel = mt_Compute_Vel(sf::Vector2f(m_Offsets[m_Current_Offset].x, m_Offsets[m_Current_Offset].y), m_Tgt->m_Speed);
        m_Tgt->m_Desired_Vel = mt_Compute_Vel(l_Diff, m_Tgt->m_Speed);
    }
    //std::cout << m_Tgt->m_Desired_Vel.x << '\t' << m_Tgt->m_Desired_Vel.y << '\n';
}

sf::Vector2f Command_MoveOffset::mt_Compute_Vel(const sf::Vector2f& offset, float speed)
{
    sf::Vector2f l_Ret(0.0f, 0.0f);
    float l_Angle;

    l_Angle = atan2(offset.y, offset.x);

    if (offset.x == 0)
    {
        l_Ret.x = 0.0f;
    }
    else
    {
        l_Ret.x = speed * cos(l_Angle);
    }

    if (offset.y == 0)
    {
        l_Ret.y = 0.0f;
    }
    else
    {
        l_Ret.y = speed * sin(l_Angle);
    }

    return l_Ret;
}





Command_MoveTo::Command_MoveTo(Creature* obj, const sf::Vector2f& tgt_pos_cell, float speed)
 :  m_Obj(obj),
    m_Start_Pos(),
    m_Tgt_Pos(tgt_pos_cell),
    m_Accumulated_Time(0.0f),
    m_Obj_Speed(speed)
{}

void Command_MoveTo::mt_Start(void)
{
    std::vector<sf::Vector2i> l_Offsets;
    m_Start_Pos = m_Obj->m_Pos;

    l_Offsets.push_back(sf::Vector2i(m_Tgt_Pos.x - m_Obj->m_Pos.x, m_Tgt_Pos.y - m_Obj->m_Pos.y));

    for (std::size_t ii = 0; ii < l_Offsets.size(); ii++)
    {
        std::cout << ii << ": " << l_Offsets[ii].x << " " << l_Offsets[ii].y << '\n';
    }

    m_Offsets.reset(new Command_MoveOffset(m_Obj, l_Offsets, m_Obj_Speed));
    m_Offsets->mt_Start();
}

void Command_MoveTo::mt_Update(float elapsed_time)
{
    m_Offsets->mt_Update(elapsed_time);
    m_Completed = m_Offsets->m_Completed;
}



Command_SetPos::Command_SetPos(Creature* obj, const sf::Vector2f& tgt_pos_cell)
 :  m_Obj(obj),
    m_Tgt_Pos(tgt_pos_cell)
{}

void Command_SetPos::mt_Start(void)
{}

void Command_SetPos::mt_Update(float elapsed_time)
{
    m_Obj->m_Pos = m_Tgt_Pos;
    m_Completed  = true;
}


Command_ShowDialog::Command_ShowDialog(const std::vector<sf::String>& text)
 :  m_Text(text)
{}

void Command_ShowDialog::mt_Start(void)
{
    Context::smt_Get().m_Dialog->mt_Show_Dialog(m_Text);
}

void Command_ShowDialog::mt_Update(float elapsed_time)
{
    (void)elapsed_time;
}





Command_SwitchMap::Command_SwitchMap(const std::string& map_id, const sf::Vector2f& player_pos)
 :  m_Map_Id(map_id),
    m_Player_Pos(player_pos)
{}

void Command_SwitchMap::mt_Start(void)
{
    Context::smt_Get().m_Engine->mt_Change_Map(m_Map_Id, m_Player_Pos);
    m_Completed = true;
}

void Command_SwitchMap::mt_Update(float elapsed_time)
{
    (void)elapsed_time;
}









Command_Choice::Command_Choice(const std::vector<DialogChoice>& choices)
 :  m_Choices(choices)
{}

void Command_Choice::mt_Start(void)
{
    Context::smt_Get().m_Dialog->mt_Show_Choice(m_Choices);
}
void Command_Choice::mt_Update(float elapsed_time)
{
    (void)elapsed_time;
}




Command_Wait::Command_Wait(float waiting_time_s)
 :  m_Waiting_Time_s(waiting_time_s)
{}

void Command_Wait::mt_Start(void)
{}

void Command_Wait::mt_Update(float elapsed_time)
{
    m_Waiting_Time_s -= elapsed_time;

    if (m_Waiting_Time_s <= 0.0f)
    {
        m_Completed = true;
    }
}




Command_Lights::Command_Lights(const sf::Color& start_light_color, const sf::Color& end_light_color, float gradient_time_s)
 :  m_Start_Color(start_light_color),
    m_End_Color(end_light_color),
    m_Gradient_Time_s(gradient_time_s)
{}

void Command_Lights::mt_Start(void)
{
    m_Accumulated_Time_s = 0.0f;
    Context::smt_Get().m_Engine->m_Script.m_Light_Ctrl = true;
}

void Command_Lights::mt_Update(float elapsed_time)
{
    sf::Color l_Color;
    m_Accumulated_Time_s += elapsed_time;
    Interpolator_Power l_Interpolator(2.0f);

    l_Color = fn_Interpolate(&l_Interpolator, m_Accumulated_Time_s, m_Gradient_Time_s, m_Start_Color, m_End_Color);

    Context::smt_Get().m_Engine->m_Sky_Color = l_Color;

    if (m_Accumulated_Time_s >= m_Gradient_Time_s)
    {
        m_Completed = true;
    }
}






Command_Camera::Command_Camera(const sf::Vector2f& tgt_pos_pix, float time_s, IInterpolator* interpolator)
 :  m_Tgt_Pos(tgt_pos_pix),
    m_Time_s(time_s),
    m_Interpolator(interpolator)
{}

void Command_Camera::mt_Start(void)
{
    m_Src_Pos = Context::smt_Get().m_Engine->mt_Get_Camera_View().getCenter();
    m_Accumulated_Time_s = 0.0f;
}

void Command_Camera::mt_Update(float elapsed_time)
{
    sf::Vector2f l_Pos;
    m_Accumulated_Time_s += elapsed_time;

    l_Pos = fn_Interpolate(m_Interpolator.get(), m_Accumulated_Time_s, m_Time_s, m_Src_Pos, m_Tgt_Pos);

    Context::smt_Get().m_Engine->mt_Set_Camera_Center(l_Pos);
    if (m_Accumulated_Time_s >= m_Time_s)
    {
        m_Completed = true;
    }
}






Command_Fight::Command_Fight(const std::map<int, std::vector<CommandFightCreature>>& fighters, std::function<void(void)> pfn_On_Victory, const std::string& music_id)
 :  m_Fighters(fighters), m_Music_Id(music_id), m_pfn_On_Victory(pfn_On_Victory)
{}

void Command_Fight::mt_Start(void)
{
    Context::smt_Get().m_Engine->mt_Start_Fight(m_Fighters, m_pfn_On_Victory, m_Music_Id);
}

void Command_Fight::mt_Update(float elapsed_time)
{
    (void)elapsed_time;
}



Command_Creature_LookAt::Command_Creature_LookAt(Creature* tgt, const sf::Vector2f& cmd, bool direction)
 :  m_Target(tgt),
    m_Cmd(cmd),
    m_Direction(direction),
    m_Other(nullptr)
{}

Command_Creature_LookAt::Command_Creature_LookAt(Creature* tgt, const Creature* other)
 :  m_Target(tgt),
    m_Cmd(),
    m_Direction(false),
    m_Other(other)
{}

void Command_Creature_LookAt::mt_Start(void)
{
    if (m_Other != nullptr)
    {
        m_Cmd = m_Other->m_Pos;
    }
}

void Command_Creature_LookAt::mt_Update(float elapsed_time)
{
    if (m_Direction)
    {
        m_Target->mt_LookDir(m_Cmd);
    }
    else
    {
        m_Target->mt_LookAt(m_Cmd);
    }
    m_Completed = true;
}



Command_ChestOpen::Command_ChestOpen(Dynamic_Chest* tgt) : m_Tgt(tgt)
{}

void Command_ChestOpen::mt_Start(void)
{
    m_Time = m_Tgt->m_Sprite_Data.m_Switch_Time_s;
    m_Tgt->m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::South;
    Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Chest_String, {0.0f, 0.0f, 0.0f}, true);
}

void Command_ChestOpen::mt_Update(float elapsed_time)
{
    m_Time -= elapsed_time;
    if (m_Time < 0.0f)
    {
        m_Time = m_Tgt->m_Sprite_Data.m_Switch_Time_s;
        if (m_Tgt->m_Sprite_Data.m_Facing_Dir == SpriteSheet_Direction::South)
        {
            m_Tgt->m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::West;
        }
        else if (m_Tgt->m_Sprite_Data.m_Facing_Dir == SpriteSheet_Direction::West)
        {
            m_Tgt->m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::East;
        }
        else if (m_Tgt->m_Sprite_Data.m_Facing_Dir == SpriteSheet_Direction::East)
        {
            m_Tgt->m_Sprite_Data.m_Facing_Dir = SpriteSheet_Direction::North;
        }
        else
        {
            m_Completed = true;
            std::cout << (int) m_Tgt->m_Sprite_Data.m_Facing_Dir << '\n';
        }
    }
}



Command_AddSkill::Command_AddSkill(Creature* tgt, const std::string& skill_id)
 :  m_Tgt(tgt),
 m_Skill_Id(skill_id)
{}

void Command_AddSkill::mt_Start(void)
{
    m_Tgt->m_Gameplay_Data.m_Skills.push_back(Context::smt_Get().m_Skills.mt_Get_Resource(m_Skill_Id));
}

void Command_AddSkill::mt_Update(float elapsed_time)
{
    m_Completed = true;
}



Command_Music::Command_Music(const std::string& music_id, float transition_time)
 :  m_Music_Id(music_id), m_Transition_Time(transition_time)
{}

void Command_Music::mt_Start(void)
{
    if (Context::smt_Get().m_Engine->mt_Get_Music() != nullptr)
    {
        m_Start_Volume = Context::smt_Get().m_Engine->mt_Get_Music()->getVolume();
    }
    m_Accumulated_Time = 0.0f;
}

void Command_Music::mt_Update(float elapsed_time)
{
    Interpolator_Linear l_Interpolator;

    m_Accumulated_Time += elapsed_time;

    if (Context::smt_Get().m_Engine->mt_Get_Music() != nullptr)
    {
        Context::smt_Get().m_Engine->mt_Get_Music()->setVolume(l_Interpolator.mt_Interpolate(m_Accumulated_Time, m_Transition_Time, m_Start_Volume, 0.0f));
    }

    if (m_Accumulated_Time >= m_Transition_Time)
    {
        Context::smt_Get().m_Engine->mt_Play_Music(m_Music_Id);
        m_Completed = true;
    }
}



void Command_End_Game::mt_Start(void)
{
    Context::smt_Get().m_Engine->mt_End_Game();
    m_Completed = true;
}

void Command_End_Game::mt_Update(float elapsed_time)
{
    m_Completed = true;
}


void Command_Set_Intro::mt_Start(void)
{
    Context::smt_Get().m_Engine->m_State = GameStateType::Choose_Class;
    m_Completed = true;
}

void Command_Set_Intro::mt_Update(float elapsed_time)
{
    m_Completed = true;
}

