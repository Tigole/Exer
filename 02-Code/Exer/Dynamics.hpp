#ifndef _DYNAMIC_HPP
#define _DYNAMIC_HPP 1

#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rendertarget.hpp>
#include <SFML/Graphics/View.hpp>
#include "SpriteSheet.hpp"
#include "Managers/ResourceManager.hpp"
#include "Skill.hpp"
#include "Item.hpp"

class Creature;

class Dynamic
{
public:
    Dynamic(const std::string& name);
    virtual ~Dynamic();

    sf::Vector2f m_Pos;
    sf::Vector2f m_Vel;
    sf::Vector2f m_Desired_Vel;
    float m_Speed;
    bool m_Solid_Map;
    bool m_Solid_Dyn;
    bool m_Friendly;
    std::string m_Name;
    sf::Vector2f m_Dim;
    float m_Hitbox_Margin;

    virtual void mt_Draw(sf::RenderTarget& target, sf::RenderStates state) = 0;
    virtual void mt_Update(float elapsed_time) = 0;
    virtual void mt_OnInteract(Creature* player) = 0;

    sf::FloatRect mt_Get_Hit_Box(void) const;
public:
    void mt_Set_Pos(const sf::Vector2f & pos);
};

struct GameplayData_Aventure
{
    GameplayData_Aventure() : m_Psy(10), m_Psy_Max(10), m_Physic_per100(50), m_Social_per100(50), m_Mental_per100(50) {}
    int m_Psy;
    int m_Psy_Max;

    int m_Physic_per100;
    int m_Social_per100;
    int m_Mental_per100;
};

struct GameplayData
{
    GameplayData() : m_Health(10), m_Health_Max(10), m_Movement(5.0f), m_Data_Aventure(), m_Skills() {}
    int m_Health;
    int m_Health_Max;

    float m_Movement;

    GameplayData_Aventure m_Data_Aventure;
    std::vector<Resource<ISkill>> m_Skills;
};

enum class CreatureState
{
    Idle,
    Moving,
    Speaking,
    Dead,
};

class DynamicManager;
class Creature;

class Creature_Control
{
public:
    Creature_Control(Creature* tgt);
    void mt_Update(float elapsed_time);

protected:
    virtual float mt_On_Decide(void) = 0;

    Creature* m_Tgt;

private:
    float m_Accumulated_Time;
    float m_Threshold;
};

class Creature_Control_AI_Square : public Creature_Control
{
public:
    Creature_Control_AI_Square(Creature* tgt);

    float mt_On_Decide(void);

    int m_Threshold_Idle_To_Move_Min_ms;
    int m_Threshold_Idle_To_Move_Max_ms;
    int m_Threshold_Move_To_Idle_Min_ms;
    int m_Threshold_Move_To_Idle_Max_ms;

    sf::FloatRect m_Square;
};

class Creature_Control_AI_Rand : public Creature_Control
{
public:
    Creature_Control_AI_Rand(Creature* tgt);

    float mt_On_Decide(void);

    int m_Threshold_Idle_To_Move_Min_ms;
    int m_Threshold_Idle_To_Move_Max_ms;
    int m_Threshold_Move_To_Idle_Min_ms;
    int m_Threshold_Move_To_Idle_Max_ms;
};

class Creature : public Dynamic
{
public:
    Creature(const std::string& name, Resource<SpriteSheetAnimator> sprite);
    Creature();

    ~Creature();

    std::unique_ptr<Creature_Control> m_AI;

    void mt_Draw(sf::RenderTarget& target, sf::RenderStates state) override;
    void mt_Update(float elapsed_time) override;
    void mt_OnInteract(Creature* player) override;

    void mt_Set_Sprite(const std::string& sprite_id);

    SpriteSheet_Direction mt_Get_Facing_Dir(void);
    void mt_LookAt(const Creature& tgt);
    void mt_LookAt(const sf::Vector2f& cell);
    void mt_LookDir(const sf::Vector2f& direction);

    GameplayData m_Gameplay_Data;
    sf::Color m_Dialog_Color;
    std::vector<sf::String> m_Dialog;
protected:
    Resource<SpriteSheetAnimator> m_Sprite;
    sf::IntRect m_Sprite_Rect;
    SpriteSheetAnimatorData m_Sprite_Data;

public:
    CreatureState m_State;
};


class Teleport : public Dynamic
{
public:
    Teleport(const sf::Vector2f& current_pos, const std::string& target_map_id, const sf::Vector2f& target_map_pos);

    void mt_Draw(sf::RenderTarget& target, sf::RenderStates state);
    void mt_Update(float elapsed_time);
    void mt_OnInteract(Creature* player) override;

    std::string m_Map_Id;
    sf::Vector2f m_Map_Pos;
};

class Dynamic_Script : public Dynamic
{
public:
    Dynamic_Script()
     :  Dynamic("")
    {
        m_Solid_Dyn = false;
        m_Solid_Map = false;
        m_Dim = {1.0f, 1.0f};
        m_Hitbox_Margin = 0.0f;
    }

    void mt_Draw(sf::RenderTarget& target, sf::RenderStates state);
    void mt_Update(float elapsed_time){}
    void mt_OnInteract(Creature* player) override;

    std::string m_Quest_Id;
};

struct ChestData
{
    ChestData() : m_Item_Id(""), m_Item_Type(ItemType::COUNT), m_Quantity(0) {}
    ChestData(const std::string& id, ItemType it, int quantity) : m_Item_Id(id), m_Item_Type(it), m_Quantity(quantity) {}
    std::string m_Item_Id;
    ItemType m_Item_Type;
    int m_Quantity;
};

class Dynamic_Chest : public Dynamic
{
public:
    Dynamic_Chest();

    void mt_Draw(sf::RenderTarget& target, sf::RenderStates state) override;
    void mt_Update(float elapsed_time) override;
    void mt_OnInteract(Creature* player) override;

    Resource<SpriteSheetAnimator> m_Sprite;
    sf::IntRect m_Sprite_Rect;
    SpriteSheetAnimatorData m_Sprite_Data;
    std::vector<ChestData> m_Items;
    bool m_Open = false;
};

class Dynamic_Light : public Dynamic
{
public:
    Dynamic_Light();

    void mt_Draw(sf::RenderTarget& target, sf::RenderStates state) override;
    void mt_Update(float elapsed_time) override;
    void mt_OnInteract(Creature* player) override;

    sf::Color m_Final_Color;
    sf::Color m_Start_Color;
    float m_Duration_s;
    bool m_Start;
    float m_Accumulated_Time_s;
};

class Dynamic_Music : public Dynamic
{
public:
    Dynamic_Music();

    void mt_Draw(sf::RenderTarget& target, sf::RenderStates state) override;
    void mt_Update(float elapsed_time) override;
    void mt_OnInteract(Creature* player) override;

    float m_Acc_Time;
    float m_Transition_Time;
    bool m_Started;
    std::string m_Music_Id;
};

#endif // _DYNAMIC_HPP
