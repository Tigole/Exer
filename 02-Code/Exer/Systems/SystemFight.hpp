#ifndef _SYSTEM_FIGHT_HPP
#define _SYSTEM_FIGHT_HPP 1

#include "../State.hpp"

#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "SystemDialog.hpp"
#include "SystemScript.hpp"
#include "SystemAnimation.hpp"
#include "../Command.hpp"

class Creature;
class ISkill;
class SystemInventory;

enum class SystemFightStates
{
    Start,
    Ordering,
    Turn,
    Done,

    COUNT,
};

enum class SystemFightFighterStates
{
    Done,
    Choose_Action,
    Choose_Skill,
    Choose_Object,
    Choose_Target,
    Make_Action,
    Move,

    Run_Away,

    COUNT,
};

enum class FighterAction
{
    Idle,
    RunAway,
    UseSkill,
    UseObject,

    COUNT,
};

std::ostream& operator<<(std::ostream& o, SystemFightStates s);
std::ostream& operator<<(std::ostream& o, SystemFightFighterStates s);

class SystemFight;

class FightLogic
{
public:
    virtual void mt_Handle_Event(sf::Event& event) = 0;
    virtual void mt_Update(float elapsed_time_s) = 0;
    virtual void mt_Draw(sf::RenderTarget& target){}

    void mt_Update_Movement(float elapsed_time_s);
    sf::Vector2f m_Movement_Previous_Pos;

    bool m_Action_Decided;
    FighterAction m_Action;
    FightCreature* m_Tgt;
    SystemFight* m_System_Fight;
    State<SystemFightFighterStates> m_State;
    SystemScript* m_Script;
    SystemDialog* m_Dialog;
    SystemAnimation* m_Animation;
    float m_Move_Distance_Left;
    int m_Actions_Remaining;
};

class FightLogic_Human : public FightLogic
{
public:
    void mt_Handle_Event(sf::Event& event) override;
    void mt_Update(float elapsed_time_s) override;
    virtual void mt_Draw(sf::RenderTarget& target) override;

    ISkill* m_Player_Skill;
    std::vector<FightCreature*> m_Targetable_Creatures;
    std::size_t m_Player_Current_Target;
    std::vector<FightCreature*> m_Player_Tgt;

    Item_Edible* m_Player_Item;


    void mt_On_RunAway(void);
    void mt_On_Skill(void);
    void mt_On_Move(void);
    void mt_On_Object(void);
    void mt_On_Pass(void);

    Interpolator_Cos m_Interpolator;
    float m_Accumulated_Time;
};

class FightAI : public FightLogic
{
public:
    void mt_Handle_Event(sf::Event& event) override {}
};

class FightAI_Dumb : public FightAI
{
public:
    void mt_Update(float elapsed_time_s) override;

    virtual ISkill* mt_Select_Skill(void);
};

class FightAI_Mage : public FightAI_Dumb
{
public:
    ISkill* mt_Select_Skill(void) override;
};




class FightCreature
{
public:
    Creature* m_Tgt;
    FightLogic* m_Logic;
    int m_Party_Id;
    int m_Actions_Count;

    FightCreature* m_Target_Move;
};




class Command_Fight_Action : public Command
{
public:
    Command_Fight_Action(FighterAction type) : Command(), m_Type(type) {}
    virtual ~Command_Fight_Action(){}

    FighterAction m_Type;
};

class Command_Fight_Use_Object : public Command_Fight_Action
{
public:
    Command_Fight_Use_Object(Item* item, Creature* tgt);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;
    Item* m_Item;
    Creature* m_Tgt;
    std::size_t m_Anim_Id;
};

struct SkillCreature
{
    FightCreature* m_Tgt;
    bool m_Success;
    bool m_Action_Done;
    std::size_t m_Anim_Id;
};

class Command_Fight_Skill : public Command_Fight_Action
{
public:
    Command_Fight_Skill(FightCreature* src, const std::vector<FightCreature*>& tgt, ISkill* skill);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    FightCreature* m_Src;
    std::vector<SkillCreature> m_Tgt;
    ISkill* m_Skill;
    float m_Accumulated_Time_s;
};

class Command_Play_Animation : public Command
{
public:
    Command_Play_Animation(const std::string& anim_id, const sf::Vector2f& position, const sf::Vector2f& dimension);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    std::string m_Anim_Id;
    sf::Vector2f m_Position;
    sf::Vector2f m_Dimension;
};

class SystemFight
{
public:
    //
    void mt_Handle_Event(sf::Event& event);
    void mt_Update(float delta_time_s);
    void mt_Draw(sf::RenderTarget& target);

    void mt_Start_Fight(const std::map<int, std::vector<CommandFightCreature>>& fighters,
                        std::function<void(void)> pfn_On_Victory,
                        const std::string current_music_id,
                        const std::string fight_music_id);
    bool mt_Is_Ended(void) const;


    void mt_Update_Ordering(float delta_time_s);
    void mt_Update_Turn(float delta_time_s);

    std::vector<FightCreature> m_Fighters;
    std::vector<std::size_t> m_Ordered_Fighters;
    std::size_t m_Current_Fighter;
    ISkill* m_Player_Skill;
    std::vector<FightCreature*> m_Targetable_Creatures;
    std::size_t m_Player_Current_Target;
    std::vector<FightCreature*> m_Player_Tgt;

    State<SystemFightStates> m_State;

    std::vector<FightCreature*> mt_Get_Targetable(const ISkill* skill, const FightCreature* src);

    FighterAction m_Player_Action;
    bool m_Player_Win;
    std::function<void(void)> m_pfn_On_Victory;

    static const std::string m_Human_Logic_String;
    static const std::string m_Dumb_Logic_String;
    static const std::string m_Boss_Logic_String;
    static const std::string m_Villager_Logic_String;

private:

    bool mt_Should_End(void) const;

    SystemDialog m_Dialog;

    SystemAnimation m_Animation;

    FightLogic_Human m_Human_Logic;
    FightAI_Mage m_Boss_Logic;
    FightAI_Dumb m_Dumb_Logic;
    FightAI_Dumb m_Villager_Logic;


private:
    SystemScript m_Script;
};

#endif // _SYSTEM_FIGHT_HPP
