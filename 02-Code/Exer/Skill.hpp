#ifndef _SKILL_HPP
#define _SKILL_HPP 1

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "SpriteSheet.hpp"

class FightCreature;

class ISkill
{
public:

    virtual bool mt_On_Test(FightCreature* src, FightCreature* tgt) = 0;
    virtual void mt_On_Action(FightCreature* src, FightCreature* tgt) = 0;

    sf::String m_Name;
    sf::String m_Description;
    std::size_t m_Tgt_Max_Count;
    float m_Distance_Max;
    std::string m_Animation_Id;
    int m_Damage;
    int m_Cost;
    Resource<SpriteSheet> m_Sprite;
    sf::Vector2f m_Sprite_Rect;
};

class Skill : public ISkill
{
public:
    bool mt_On_Test(FightCreature* src, FightCreature* tgt) override;
    void mt_On_Action(FightCreature* src, FightCreature* tgt) override;
};

class Skill_Physical : public ISkill
{
public:
    bool mt_On_Test(FightCreature* src, FightCreature* tgt) override;
    void mt_On_Action(FightCreature* src, FightCreature* tgt) override;
};

class Skill_FireBall : public ISkill
{
public:
    bool mt_On_Test(FightCreature* src, FightCreature* tgt) override;
    void mt_On_Action(FightCreature* src, FightCreature* tgt) override;
};

#endif // _SKILL_HPP
