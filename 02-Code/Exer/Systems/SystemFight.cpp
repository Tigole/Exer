#include "SystemFight.hpp"

#include <SFML/Graphics.hpp>
#include "../Context.hpp"
#include "../GameEngine.hpp"

std::ostream& operator<<(std::ostream& o, SystemFightStates s)
{
    switch(s)
    {
    case SystemFightStates::Ordering: o << "Ordering"; break;
    case SystemFightStates::Turn: o << "Turn"; break;
    case SystemFightStates::Done: o << "Done"; break;
    }
    return o;
}

std::ostream& operator<<(std::ostream& o, SystemFightFighterStates s)
{
    switch(s)
    {
    case SystemFightFighterStates::Done: o << "Done"; break;
    case SystemFightFighterStates::Choose_Action: o << "Choose_Action"; break;
    case SystemFightFighterStates::Make_Action: o << "Make_Action"; break;
    case SystemFightFighterStates::Choose_Object: o << "Choose_Object"; break;
    case SystemFightFighterStates::Choose_Skill: o << "Choose_Skill"; break;
    case SystemFightFighterStates::Choose_Target: o << "Choose_Target"; break;
    case SystemFightFighterStates::Move: o << "Move"; break;
    case SystemFightFighterStates::Run_Away: o << "Run_Away"; break;
    }
    return o;
}


void FightLogic_Human::mt_Handle_Event(sf::Event& event)
{
    EventType l_Type = fn_EventType(event);

    if (m_State.m_Current == SystemFightFighterStates::Choose_Skill)
    {
        if (fn_EventType_Validate(event))
        {
            m_Player_Skill = Context::smt_Get().m_Engine->m_Skill_State.mt_Get_Selected_Skill();
            m_State = SystemFightFighterStates::Choose_Target;
            m_Player_Current_Target = 0;
            m_Player_Tgt.resize(0);

            m_Targetable_Creatures = m_System_Fight->mt_Get_Targetable(m_Player_Skill, m_Tgt);
        }
        else if (fn_EventType_Cancel(event))
        {
            m_State.m_Previous = m_State.m_Current;
            m_State.m_Current = SystemFightFighterStates::Choose_Action;
        }
        else
        {
            Context::smt_Get().m_Engine->m_Skill_State.mt_Handle_Event(event);
        }
    }
    else if (m_State.m_Current == SystemFightFighterStates::Choose_Target)
    {
        if ((l_Type == EventType::Released_Down) || (l_Type == EventType::Released_Right))
        {
            if (m_Player_Current_Target < (m_Targetable_Creatures.size()-1))
                m_Player_Current_Target++;
            else
                m_Player_Current_Target = 0;
        }
        else if ((l_Type == EventType::Released_Up) || (l_Type == EventType::Released_Left))
        {
            if (m_Player_Current_Target > 0)
                m_Player_Current_Target--;
            else
                m_Player_Current_Target = m_Targetable_Creatures.size() - 1;
        }
        else if (l_Type == EventType::Validate)
        {
            m_Player_Tgt.push_back(m_Targetable_Creatures[m_Player_Current_Target]);
            if (m_Player_Tgt.size() >= m_Player_Skill->m_Tgt_Max_Count)
            {
                m_Script->mt_Add_Command(new Command_Fight_Skill(m_Tgt, m_Player_Tgt, m_Player_Skill));
                m_State = SystemFightFighterStates::Make_Action;
            }
        }
        else if (l_Type == EventType::Cancel)
        {
            m_State.m_Previous = m_State.m_Current;
            m_State.m_Current = SystemFightFighterStates::Choose_Action;
        }
    }
    else if (m_State.m_Current == SystemFightFighterStates::Choose_Object)
    {
        if (fn_EventType_Validate(event))
        {
            m_Player_Item = dynamic_cast<Item_Edible*>(Context::smt_Get().m_Engine->m_Inventory_State.mt_Get_Selected_Item());

            if (m_Player_Item != nullptr)
            {
                Context::smt_Get().m_Engine->m_Inventory_State.mt_Lock_Type(ItemType::COUNT);

                m_Script->mt_Add_Command(new Command_Fight_Use_Object(m_Player_Item,
                                                                      m_Tgt->m_Tgt));

                m_State = SystemFightFighterStates::Make_Action;
            }
            else
            {
                Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Cancel_String, {0.0f, 0.0f, 0.0f}, true);
            }
        }
        else if (fn_EventType_Cancel(event))
        {
            Context::smt_Get().m_Engine->m_Inventory_State.mt_Lock_Type(ItemType::COUNT);
            m_State.m_Previous = m_State.m_Current;
            m_State.m_Current = SystemFightFighterStates::Choose_Action;
        }
        else
        {
            Context::smt_Get().m_Engine->m_Inventory_State.mt_Handle_Event(event);
        }
    }
    else if (m_State.m_Current == SystemFightFighterStates::Move)
    {
        if (l_Type == EventType::Released_Left || l_Type == EventType::Released_Right)
        {
            m_Tgt->m_Tgt->m_Desired_Vel.x = 0.0f;
        }
        else if (l_Type == EventType::Released_Down || l_Type == EventType::Released_Up)
        {
            m_Tgt->m_Tgt->m_Desired_Vel.y = 0.0f;
        }
        else if (l_Type == EventType::Pressed_Down)
        {
            m_Tgt->m_Tgt->m_Desired_Vel.y = m_Tgt->m_Tgt->m_Speed;
        }
        else if (l_Type == EventType::Pressed_Up)
        {
            m_Tgt->m_Tgt->m_Desired_Vel.y = -m_Tgt->m_Tgt->m_Speed;
        }
        else if (l_Type == EventType::Pressed_Left)
        {
            m_Tgt->m_Tgt->m_Desired_Vel.x = -m_Tgt->m_Tgt->m_Speed;
        }
        else if (l_Type == EventType::Pressed_Right)
        {
            m_Tgt->m_Tgt->m_Desired_Vel.x = m_Tgt->m_Tgt->m_Speed;
        }
        else if (l_Type == EventType::Validate)
        {
            m_State = SystemFightFighterStates::Done;
        }
    }
    else
    {
        //
    }


    m_Dialog->mt_Handle_Event(event);
}

void FightLogic_Human::mt_Update(float elapsed_time_s)
{
    switch(m_State.m_Current)
    {
    case SystemFightFighterStates::Choose_Action:
        if (m_State.mt_On_Entry())
        {
            std::vector<DialogChoice> l_Choices;

            l_Choices.push_back(DialogChoice("Attaquer", &FightLogic_Human::mt_On_Skill, this));
            l_Choices.push_back(DialogChoice("Se déplacer", &FightLogic_Human::mt_On_Move, this));
            if (Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item_Count(ItemType::Edible) > 0)
            {
                l_Choices.push_back(DialogChoice("Objet", &FightLogic_Human::mt_On_Object, this));
            }
            //l_Choices.push_back(DialogChoice("Fuir", &FightLogic_Human::mt_On_RunAway, this));

            m_Dialog->mt_Show_Choice(l_Choices);

            m_State.m_Previous = m_State.m_Current;
        }
        m_Dialog->mt_Update(elapsed_time_s);
        break;
    case SystemFightFighterStates::Choose_Target:
        break;
    case SystemFightFighterStates::Choose_Skill:
        Context::smt_Get().m_Engine->m_Skill_State.mt_Update(elapsed_time_s);
        break;
    case SystemFightFighterStates::Make_Action:
        if (m_Script->m_User_Ctrl)
        {
            m_State = SystemFightFighterStates::Done;
        }
        break;
    case SystemFightFighterStates::Move:
        {
        sf::Vector2f l_New_Pos;

        l_New_Pos = m_Tgt->m_Tgt->m_Pos;

        m_Move_Distance_Left -= std::sqrt(std::pow(l_New_Pos.x - m_Movement_Previous_Pos.x, 2.0f) + std::pow(l_New_Pos.y - m_Movement_Previous_Pos.y , 2.0f));

        if (m_Move_Distance_Left <= 0.0f)
        {
            m_Tgt->m_Tgt->m_Desired_Vel = {0.0f, 0.0f};
            m_State = SystemFightFighterStates::Done;
        }

        m_Movement_Previous_Pos = l_New_Pos;

        }
        break;
    case SystemFightFighterStates::Choose_Object:
        Context::smt_Get().m_Engine->m_Inventory_State.mt_Update(elapsed_time_s);
        break;
    case SystemFightFighterStates::Done:
        if (m_Actions_Remaining > 0)
        {
            m_State = SystemFightFighterStates::Choose_Action;
        }
        else
        {
            m_State.m_Previous = m_State.m_Current;
        }
        break;
    case SystemFightFighterStates::COUNT:
    default:
        throw std::runtime_error("SystemFight.m_Fighter_State.m_Current");
        break;
    }
}

void FightLogic_Human::mt_Draw(sf::RenderTarget& target)
{
    if (m_State == SystemFightFighterStates::Choose_Target)
    {
        sf::Vector2f l_Pos(m_Targetable_Creatures[m_Player_Current_Target]->m_Tgt->m_Pos);
        sf::CircleShape l_Skill_Distance;

        l_Pos = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Pos);

        Context::smt_Get().m_Engine->mt_Set_Camera_Center(l_Pos);

        sf::RectangleShape l_Rect;

        l_Rect.setFillColor(sf::Color(255, 255, 255, 127+32));
        l_Rect.setPosition(l_Pos);
        l_Rect.setSize(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel());

        target.draw(l_Rect);

        l_Pos = m_Tgt->m_Tgt->m_Pos + sf::Vector2f(0.5f, 0.5f);
        l_Pos = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Pos);

        l_Skill_Distance.setPointCount(60);
        l_Skill_Distance.setPosition(l_Pos);
        l_Skill_Distance.setRadius(m_Player_Skill->m_Distance_Max * Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel().x);
        l_Skill_Distance.setFillColor(sf::Color::Transparent);
        l_Skill_Distance.setOutlineColor(sf::Color::White);
        l_Skill_Distance.setOutlineThickness(3.0f);
        l_Skill_Distance.setOrigin(l_Skill_Distance.getRadius(), l_Skill_Distance.getRadius());

        target.draw(l_Skill_Distance);
    }


    if (m_State == SystemFightFighterStates::Choose_Action)
    {
        target.setView(target.getDefaultView());
        m_Dialog->mt_Draw(target);
    }
    else if (m_State == SystemFightFighterStates::Choose_Target)
    {
        //
    }
    else if (m_State == SystemFightFighterStates::Choose_Skill)
    {
        sf::RectangleShape l_Rect;

        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setSize({target.getSize().x, target.getSize().y});

        target.setView(target.getDefaultView());
        //target.draw(l_Rect);

        Context::smt_Get().m_Engine->m_Skill_State.mt_Draw(target);
    }
    else if (m_State == SystemFightFighterStates::Choose_Object)
    {
        target.setView(target.getDefaultView());
        Context::smt_Get().m_Engine->m_Inventory_State.mt_Draw(target);
    }
    else if (m_State == SystemFightFighterStates::Move)
    {
        sf::CircleShape l_Distance_Left;

        l_Distance_Left.setRadius(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(sf::Vector2f(m_Move_Distance_Left, 1.0f)).x);
        l_Distance_Left.setFillColor(sf::Color::Transparent);
        l_Distance_Left.setOutlineColor(sf::Color::Black);
        l_Distance_Left.setOutlineThickness(3.0f);
        l_Distance_Left.setOrigin(l_Distance_Left.getRadius(), l_Distance_Left.getRadius());

        l_Distance_Left.setPosition(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(m_Tgt->m_Tgt->m_Pos + sf::Vector2f(0.5f, 0.5f)));

        target.draw(l_Distance_Left);
    }
}


void FightLogic_Human::mt_On_RunAway(void)
{
    m_State = SystemFightFighterStates::Run_Away;
}

void FightLogic_Human::mt_On_Skill(void)
{
    m_State = SystemFightFighterStates::Choose_Skill;
    m_Actions_Remaining -= 2;
}

void FightLogic_Human::mt_On_Move(void)
{
    m_Move_Distance_Left = 5.0f;
    m_State = SystemFightFighterStates::Move;
    m_Movement_Previous_Pos = m_Tgt->m_Tgt->m_Pos;
    m_Actions_Remaining -= 1;
}

void FightLogic_Human::mt_On_Object(void)
{
    m_State = SystemFightFighterStates::Choose_Object;
    Context::smt_Get().m_Engine->m_Inventory_State.mt_Lock_Type(ItemType::Edible);
    m_Actions_Remaining -= 2;
}



void FightAI_Dumb::mt_Update(float elapsed_time_s)
{
    if (m_State == SystemFightFighterStates::Choose_Action)
    {
        std::vector<FightCreature*> l_Targetable;
        std::vector<FightCreature*> l_Tgt;
        ISkill* l_Skill(nullptr);

        l_Skill = mt_Select_Skill();

        l_Targetable = m_System_Fight->mt_Get_Targetable(l_Skill, m_Tgt);
        for (std::size_t ii = 0; (ii < l_Targetable.size()) && (l_Tgt.size() < l_Skill->m_Tgt_Max_Count); ii++)
        {
            if (l_Targetable[ii]->m_Party_Id != m_Tgt->m_Party_Id)
            {
                l_Tgt.push_back(l_Targetable[ii]);
            }
        }

        if (l_Tgt.size() != 0)
        {
            m_Script->mt_Add_Command(new Command_Fight_Skill(m_Tgt, l_Tgt, l_Skill));
            m_State = SystemFightFighterStates::Make_Action;
            m_Actions_Remaining -= 2;
        }
        else
        {
            m_State = SystemFightFighterStates::Done;
            m_Actions_Remaining -= 2;
        }
    }
    else if (m_State == SystemFightFighterStates::Make_Action)
    {
        if (m_Script->m_User_Ctrl)
        {
            m_State = SystemFightFighterStates::Done;
        }
    }
    else if (m_State == SystemFightFighterStates::Done)
    {
        if (m_Actions_Remaining > 0)
        {
            m_State = SystemFightFighterStates::Choose_Action;
        }
        else
        {
            m_State.m_Previous = m_State.m_Current;
        }
    }
}

ISkill* FightAI_Dumb::mt_Select_Skill(void)
{
    ISkill* l_Ret(nullptr);

    l_Ret = m_Tgt->m_Tgt->m_Gameplay_Data.m_Skills[0].m_Resource;

    return l_Ret;
}










Command_Fight_Use_Object::Command_Fight_Use_Object(Item* item, Creature* tgt)
 :  Command_Fight_Action(FighterAction::UseObject), m_Item(item), m_Tgt(tgt)
{}

void Command_Fight_Use_Object::mt_Start(void)
{
    if (m_Item != nullptr)
    {
        m_Item->mt_OnUse(m_Tgt);
    }
}

void Command_Fight_Use_Object::mt_Update(float elapsed_time)
{
    m_Completed = true;
}




Command_Fight_Skill::Command_Fight_Skill(FightCreature* src, const std::vector<FightCreature*>& tgt, ISkill* skill)
 :  Command_Fight_Action(FighterAction::UseSkill),
    m_Src(src),
    m_Tgt(),
    m_Skill(skill)
{
    m_Tgt.resize(tgt.size());
    for (std::size_t ii = 0; ii < tgt.size(); ii++)
    {
        m_Tgt[ii].m_Action_Done = false;
        m_Tgt[ii].m_Success = false;
        m_Tgt[ii].m_Tgt = tgt[ii];
    }
}

void Command_Fight_Skill::mt_Start(void)
{
    std::vector<IAnimation*> l_Animations;
    Tileset* l_Tileset = Context::smt_Get().m_Engine->m_Map->m_Tileset.m_Resource;
    SystemAnimation* l_Anims = Context::smt_Get().m_System_Animation;

    m_Accumulated_Time_s = 0.0f;

    if (m_Tgt.size() == 1)
    {
        m_Src->m_Tgt->mt_LookAt(*m_Tgt[0].m_Tgt->m_Tgt);
    }

    m_Src->m_Tgt->m_Gameplay_Data.m_Data_Aventure.m_Psy -= m_Skill->m_Cost;

    for (std::size_t ii = 0; ii < m_Tgt.size(); ii++)
    {
        m_Tgt[ii].m_Success = m_Skill->mt_On_Test(m_Src, m_Tgt[ii].m_Tgt);

        if (m_Tgt[ii].m_Success == true)
        {
            l_Animations.push_back(new Animation(l_Tileset->mt_Cell_To_Pixel(m_Tgt[ii].m_Tgt->m_Tgt->m_Pos + sf::Vector2f(0.5f, 0.5f)),
                                                 l_Tileset->mt_Cell_To_Pixel(sf::Vector2f(2.0f, 2.0f)),
                                                 Context::smt_Get().m_Animations.mt_Get_Resource(m_Skill->m_Animation_Id)));

            m_Tgt[ii].m_Anim_Id = l_Anims->mt_Create_List(false);

            l_Anims->mt_Add_Animation(m_Tgt[ii].m_Anim_Id, l_Animations);

            l_Animations.resize(0);
        }
    }
}

void Command_Fight_Skill::mt_Update(float elapsed_time)
{
    m_Completed = true;
    for (std::size_t ii = 0; (ii < m_Tgt.size()) && (m_Completed == true); ii++)
    {
        if (m_Tgt[ii].m_Success == true)
        {
            if (Context::smt_Get().m_System_Animation->mt_Is_Ended(m_Tgt[ii].m_Anim_Id) == false)
            {
                m_Completed = false;
            }
            else if (m_Tgt[ii].m_Action_Done == false)
            {
                m_Tgt[ii].m_Action_Done = true;
                m_Skill->mt_On_Action(m_Src, m_Tgt[ii].m_Tgt);
            }
        }
    }
}



Command_Play_Animation::Command_Play_Animation(const std::string& anim_id, const sf::Vector2f& position, const sf::Vector2f& dimension)
 :  m_Anim_Id(anim_id),
    m_Position(position),
    m_Dimension(dimension)
{}

void Command_Play_Animation::mt_Start(void)
{
//    Context::smt_Get().m_System_Animation->mt_Add_Animation(new Animation_Skill(m_Position, m_Dimension));
    m_Completed = true;
}

void Command_Play_Animation::mt_Update(float elapsed_time)
{
    //
}


const std::string SystemFight::m_Human_Logic_String = "Human";
const std::string SystemFight::m_Dumb_Logic_String = "Dumb";
const std::string SystemFight::m_Boss_Logic_String = "Boss";
const std::string SystemFight::m_Villager_Logic_String = "Villager";

void SystemFight::mt_Handle_Event(sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Q && event.key.control)
    {
        m_State = SystemFightStates::Done;
    }
    else if (m_Script.m_User_Ctrl)
    {
        if (m_State.m_Current == SystemFightStates::Turn)
        {
            m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->mt_Handle_Event(event);
        }
    }
}

void SystemFight::mt_Update(float delta_time_s)
{
    m_Script.mt_Process_Command(delta_time_s);
    m_Animation.mt_Update(delta_time_s);

    switch (m_State.m_Current)
    {
    case SystemFightStates::Ordering:
        mt_Update_Ordering(delta_time_s);
        break;
    case SystemFightStates::Turn:
        mt_Update_Turn(delta_time_s);
        break;
    case SystemFightStates::Done:
        if (1)
        {
            m_Player_Win = true;
        }
        else
        {
            m_Player_Win = false;
        }
        m_State.m_Previous = m_State.m_Current;
        break;
    case SystemFightStates::COUNT:
    default:
        throw std::runtime_error("Exception: SystemFight.m_State.m_Current");
        break;
    }
}

void SystemFight::mt_Draw(sf::RenderTarget& target)
{
    sf::RectangleShape l_Edge;
    sf::RectangleShape l_Fill;

    l_Edge.setFillColor(sf::Color::Transparent);
    l_Edge.setOutlineColor(sf::Color::Black);
    l_Edge.setOutlineThickness(2.0f);
    l_Edge.setSize({32.0f, 3.0f});
    l_Fill.setSize(l_Edge.getSize());

    /// Life/Psy bars
    for (std::size_t ii = 0; ii < m_Ordered_Fighters.size(); ii++)
    {
        FightCreature& c(m_Fighters[m_Ordered_Fighters[ii]]);
        sf::Vector2f l_Pos(c.m_Tgt->m_Pos);
        float l_Ratio;

        l_Pos = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Pos);

        l_Ratio = c.m_Tgt->m_Gameplay_Data.m_Data_Aventure.m_Psy / (float)c.m_Tgt->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max;
        l_Fill.setFillColor(sf::Color(255, 0, 255));
        l_Fill.setScale(l_Ratio, 1.0f);
        l_Edge.setPosition(l_Pos + sf::Vector2f(0.0f, -6.0f));
        l_Fill.setPosition(l_Pos + sf::Vector2f(0.0f, -6.0f));
        target.draw(l_Edge);
        target.draw(l_Fill);

        l_Ratio = c.m_Tgt->m_Gameplay_Data.m_Health / (float)c.m_Tgt->m_Gameplay_Data.m_Health_Max;
        l_Fill.setFillColor(sf::Color::Red);
        l_Fill.setScale(l_Ratio, 1.0f);
        l_Edge.setPosition(l_Pos + sf::Vector2f(0.0f, -12.0f));
        l_Fill.setPosition(l_Pos + sf::Vector2f(0.0f, -12.0f));
        target.draw(l_Edge);
        target.draw(l_Fill);
    }

    m_Animation.mt_Draw(target);

    if (m_State == SystemFightStates::Turn)
    {
        m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->mt_Draw(target);
    }

    target.setView(target.getDefaultView());
    /*sf::Font* l_Font = Context::smt_Get().m_Dialog->m_Dialog_Font.m_Resource;
    sf::Text l_Text;
    l_Text.setString("Fight!");
    l_Text.setColor(sf::Color(102, 0, 255));
    l_Text.setFont(*l_Font);
    target.draw(l_Text);*/
}




void SystemFight::mt_Update_Ordering(float delta_time_s)
{
    std::map<int, std::size_t, std::less<int>> l_Ordering;

    m_Ordered_Fighters.resize(0);
    for (std::size_t ii = 0; ii < m_Fighters.size(); ii++)
    {
        if (m_Fighters[ii].m_Tgt->m_Gameplay_Data.m_Health > 0)
        {
            int l_Result;
            /// Make test
            do
            {
                l_Result = ii;
            }
            while(l_Ordering.find(l_Result) != l_Ordering.end());
            l_Ordering[l_Result] = ii;
        }
    }

    for (auto o : l_Ordering)
    {
        m_Ordered_Fighters.push_back(o.second);
    }

    m_Current_Fighter = 0;
    m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_State = SystemFightFighterStates::Choose_Action;
    m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_Actions_Remaining = m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Actions_Count;
    m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_Tgt = &m_Fighters[m_Ordered_Fighters[m_Current_Fighter]];

    m_State = SystemFightStates::Turn;
}

void SystemFight::mt_Update_Turn(float delta_time_s)
{
    if (m_Current_Fighter >= m_Ordered_Fighters.size())
    {
        m_State = SystemFightStates::Ordering;
    }
    else
    {
        m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->mt_Update(delta_time_s);

        if (    (m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_State.m_Previous == m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_State.m_Current)
            &&  (m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_State.m_Current == SystemFightFighterStates::Done))
        {
            if (mt_Should_End())
            {
                m_State = SystemFightStates::Done;
            }
            else
            {
                bool l_b_Search = true;
                do
                {
                    m_Current_Fighter++;
                    if (    (   (m_Current_Fighter < m_Ordered_Fighters.size())
                            &&  (m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Tgt->m_Gameplay_Data.m_Health > 0))
                        ||  (m_Current_Fighter >= m_Ordered_Fighters.size()))
                    {
                        l_b_Search = false;
                    }
                } while (l_b_Search);

                if (m_Current_Fighter >= m_Ordered_Fighters.size())
                {
                    m_State = SystemFightStates::Ordering;
                }
                else
                {
                    m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_State = SystemFightFighterStates::Choose_Action;
                    m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_Actions_Remaining = m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Actions_Count;
                    m_Fighters[m_Ordered_Fighters[m_Current_Fighter]].m_Logic->m_Tgt = &m_Fighters[m_Ordered_Fighters[m_Current_Fighter]];
                }
            }
        }
    }
}



void SystemFight::mt_Start_Fight(const std::map<int, std::vector<CommandFightCreature>>& fighters,
                                 std::function<void(void)> pfn_On_Victory,
                                 const std::string current_music_id,
                                 const std::string fight_music_id)
{
    int l_Party_Id(1);

    m_Fighters.resize(0);
    for (auto& f : fighters)
    {
        l_Party_Id = f.first;
        for (std::size_t ii = 0; ii < f.second.size(); ii++)
        {
            FightCreature l_c;

            l_c.m_Party_Id = l_Party_Id;
            l_c.m_Tgt = f.second[ii].m_Tgt;
            l_c.m_Actions_Count = f.second[ii].m_Actions_Count;
            if (f.second[ii].m_Logic_Id == m_Human_Logic_String)
            {
                l_c.m_Logic = &m_Human_Logic;
            }
            else if (f.second[ii].m_Logic_Id == m_Boss_Logic_String)
            {
                l_c.m_Logic = &m_Boss_Logic;
            }
            else if (f.second[ii].m_Logic_Id == m_Villager_Logic_String)
            {
                l_c.m_Logic = &m_Villager_Logic;
            }
            else
            {
                l_c.m_Logic = &m_Dumb_Logic;
            }

            m_Fighters.push_back(l_c);
        }
    }

    m_pfn_On_Victory = pfn_On_Victory;

    m_State = SystemFightStates::Ordering;

    m_Current_Fighter = 0;

    m_Dialog.m_Dialog_Font = Context::smt_Get().m_Fonts.mt_Get_Resource("BOOKOS");
    Context::smt_Get().m_System_Animation = &m_Animation;
    Context::smt_Get().m_Engine->m_Fight_State.m_Previous_Music_Id = current_music_id;
    m_Script.mt_Add_Command(new Command_Music("", 0.5f));
    m_Script.mt_Add_Command(new Command_Music(fight_music_id, 0.5f));

    m_Human_Logic.m_System_Fight = this;
    m_Human_Logic.m_Script = &m_Script;
    m_Human_Logic.m_Dialog = &m_Dialog;
    m_Human_Logic.m_Animation = &m_Animation;

    m_Dumb_Logic.m_System_Fight = this;
    m_Dumb_Logic.m_Script = &m_Script;
    m_Dumb_Logic.m_Dialog = &m_Dialog;
    m_Dumb_Logic.m_Animation = &m_Animation;

    m_Villager_Logic.m_System_Fight = this;
    m_Villager_Logic.m_Script = &m_Script;
    m_Villager_Logic.m_Dialog = &m_Dialog;
    m_Villager_Logic.m_Animation = &m_Animation;

    m_Boss_Logic.m_System_Fight = this;
    m_Boss_Logic.m_Script = &m_Script;
    m_Boss_Logic.m_Dialog = &m_Dialog;
    m_Boss_Logic.m_Animation = &m_Animation;

    m_Current_Fighter = 0;
}

bool SystemFight::mt_Is_Ended(void) const
{
    return m_State.m_Previous == SystemFightStates::Done;
}



std::vector<FightCreature*> SystemFight::mt_Get_Targetable(const ISkill* skill, const FightCreature* src)
{
    std::vector<FightCreature*> l_Ret;

    for (std::size_t ii = 0; ii < m_Ordered_Fighters.size(); ii++)
    {
        float l_Distance;

        auto l_tmp = m_Fighters[m_Ordered_Fighters[ii]].m_Tgt;

        l_Distance = std::sqrt(std::pow(m_Fighters[m_Ordered_Fighters[ii]].m_Tgt->m_Pos.x - src->m_Tgt->m_Pos.x, 2.0f)
                               + std::pow(m_Fighters[m_Ordered_Fighters[ii]].m_Tgt->m_Pos.y - src->m_Tgt->m_Pos.y, 2.0f));

        if (l_Distance <= skill->m_Distance_Max)
        {
            l_Ret.push_back(&m_Fighters[m_Ordered_Fighters[ii]]);
        }
    }

    return l_Ret;
}

bool SystemFight::mt_Should_End(void) const
{
    bool l_b_Ret(false);
    std::map<int, int> l_Parties_Alive;

    for (auto f : m_Fighters)
    {
        if (f.m_Tgt->m_Gameplay_Data.m_Health > 0)
        {
            l_Parties_Alive[f.m_Party_Id]++;
        }

        if (l_Parties_Alive.size() > 1)
        {
            break;
        }
    }

    l_b_Ret = (l_Parties_Alive.size() == 1);

    return l_b_Ret;
}
