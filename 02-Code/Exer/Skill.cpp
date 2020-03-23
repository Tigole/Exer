#include "Skill.hpp"
#include "Dynamics.hpp"
#include "Context.hpp"
#include "GameEngine.hpp"

bool Skill::mt_On_Test(FightCreature* src, FightCreature* tgt)
{
    return true;
}

void Skill::mt_On_Action(FightCreature* src, FightCreature* tgt)
{
    tgt->m_Tgt->m_Gameplay_Data.m_Health -= m_Damage;

    tgt->m_Tgt->m_Gameplay_Data.m_Health = std::max(tgt->m_Tgt->m_Gameplay_Data.m_Health, 0);
}

bool Skill_Physical::mt_On_Test(FightCreature* src, FightCreature* tgt)
{
    return false;
}

void Skill_Physical::mt_On_Action(FightCreature* src, FightCreature* tgt)
{
}


bool Skill_FireBall::mt_On_Test(FightCreature* src, FightCreature* tgt)
{

    return false;
}

void Skill_FireBall::mt_On_Action(FightCreature* src, FightCreature* tgt)
{
}
