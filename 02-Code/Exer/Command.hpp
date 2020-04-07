#ifndef _COMMAND_HPP
#define _COMMAND_HPP 1

#include <list>
#include <memory>

#include "Dynamics.hpp"
#include "Interpolation.hpp"
#include "Systems/SystemDialog.hpp"

class SystemDialog;
class SystemScript;

class Command
{
public:
    Command();
    virtual ~Command();

    bool m_Started;
    bool m_Completed;

    bool m_Check_Dyn_Collision;

    virtual void mt_Start(void) = 0;
    virtual void mt_Update(float elapsed_time) = 0;
};



class Command_MoveOffset : public Command
{
public:
    Command_MoveOffset(Creature* tgt, const std::vector<sf::Vector2f>& offsets, float speed, bool check_dyn_collision);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    sf::Vector2f mt_Compute_Vel(const sf::Vector2f& offset, float speed);

    Creature* m_Tgt;
    std::vector<sf::Vector2f> m_Offsets;
    std::size_t m_Current_Offset;
    sf::Vector2f m_Old_Pos;
    bool m_X_Done;
    bool m_Y_Done;

    bool m_X_Collision;
    bool m_Y_Collision;

    float m_Obj_Speed;
    float m_Obj_Init_Speed;
};



class Command_MoveTo : public Command
{
public:
    Command_MoveTo(Creature* obj, const sf::Vector2f& tgt_pos_cell, float speed, bool check_dyn_collision);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    Creature* m_Obj;
    sf::Vector2f m_Start_Pos;
    sf::Vector2f m_Tgt_Pos;
    float m_Accumulated_Time;

    std::unique_ptr<Command_MoveOffset> m_Offsets;

    float m_Obj_Speed;
    float m_Obj_Init_Speed;
};




class Command_SetPos : public Command
{
public:
    Command_SetPos(Creature* obj, const sf::Vector2f& tgt_pos_cell);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    Creature* m_Obj;
    sf::Vector2f m_Tgt_Pos;
};



class Command_ShowDialog : public Command
{
public:
    Command_ShowDialog(const std::vector<sf::String>& text);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    std::vector<sf::String> m_Text;
};

class Command_SwitchMap : public Command
{
public:
    Command_SwitchMap(const std::string& map_id, const sf::Vector2f& player_pos);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    std::string m_Map_Id;
    sf::Vector2f m_Player_Pos;
};

class Command_Choice : public Command
{
public:
    Command_Choice(const std::vector<DialogChoice>& choices);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    std::vector<DialogChoice> m_Choices;
};

class Command_Wait : public Command
{
public:
    Command_Wait(float waiting_time_s);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    float m_Waiting_Time_s;
};

class Command_Lights : public Command
{
public:
    Command_Lights(const sf::Color& start_light_color, const sf::Color& end_light_color, float gradient_time_s);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    sf::Color m_Start_Color;
    sf::Color m_End_Color;
    float m_Gradient_Time_s;
    float m_Accumulated_Time_s;
};

class Command_Camera : public Command
{
public:
    Command_Camera(const sf::Vector2f& tgt_pos_pix, float time_s, IInterpolator* interpolator, bool hold_position);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    sf::Vector2f m_Src_Pos;
    sf::Vector2f m_Tgt_Pos;
    float m_Time_s;
    float m_Accumulated_Time_s;
    std::unique_ptr<IInterpolator> m_Interpolator;
    bool m_Hold_Position;
};

class Command_Camera_Creature : public Command
{
public:
    Command_Camera_Creature(const Creature* tgt, float time_s, IInterpolator* interpolator);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    sf::Vector2f m_Src_Pos;
    sf::Vector2f m_Tgt_Pos;
    const Creature* m_Tgt;
    float m_Time_s;
    float m_Accumulated_Time_s;
    std::unique_ptr<IInterpolator> m_Interpolator;
};



struct CommandFightCreature
{
    Creature* m_Tgt;
    std::string m_Logic_Id;
    int m_Actions_Count;
};

class Command_Fight : public Command
{
public:
    Command_Fight(const std::map<int, std::vector<CommandFightCreature>>& fighters, std::function<void(void)> pfn_On_Victory, const std::string& music_id);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    const std::map<int, std::vector<CommandFightCreature>> m_Fighters;
    std::string m_Music_Id;
    std::function<void(void)> m_pfn_On_Victory;
};


class Command_Creature_LookAt : public Command
{
public:
    Command_Creature_LookAt(Creature* tgt, const sf::Vector2f& cmd, bool direction);
    Command_Creature_LookAt(Creature* tgt, const Creature* other);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    Creature* m_Target;
    sf::Vector2f m_Cmd;
    bool m_Direction;
    const Creature* m_Other;
};

class Command_Creature_Set_State : public Command
{
public:
    Command_Creature_Set_State(Creature* tgt, CreatureState new_state);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    Creature* m_Tgt;
    CreatureState m_State;
};

class Command_ChestOpen : public Command
{
public:
    Command_ChestOpen(Dynamic_Chest* tgt);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    Dynamic_Chest* m_Tgt;
    float m_Time;
};

void fn_Command_Give_Items(SystemScript* script, const std::vector<ChestData>& items);


class Command_AddSkill : public Command
{
public:
    Command_AddSkill(Creature* tgt, const std::string& skill_id);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    Creature* m_Tgt;
    std::string m_Skill_Id;
};



class Command_Music : public Command
{
public:
    Command_Music(const std::string& music_id, float transition_time);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    std::string m_Music_Id;
    float m_Transition_Time;
    float m_Accumulated_Time;
    float m_Start_Volume;
};

class Command_Sound : public Command
{
public:
    Command_Sound(const std::string& sound_id, const sf::Vector3f& pos, bool relative);

    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;

    std::string m_Sound_Id;
    sf::Vector3f m_Pos;
    bool m_Relative;
    float m_Duration;
    float m_Acc_Time;
};



class Command_End_Game : public Command
{
public:
    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;
};

class Command_Set_Intro : public Command
{
public:
    void mt_Start(void) override;
    void mt_Update(float elapsed_time) override;
};


#endif // _COMMAND_HPP
