#ifndef _QUEST_HPP
#define _QUEST_HPP 1

#include "Dynamics.hpp"

enum class Quest_Nature
{
    Talking,
    Attacking,
    Killing,
    Walking,
};

class Quest
{
public:
    Quest() : m_Quest_Id(), m_Completed(false){}
    virtual ~Quest(){}

    virtual bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature) = 0;
    virtual bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id) = 0;
    virtual void mt_Get_Description(std::vector<std::vector<sf::String>>& description) = 0;

    std::string m_Quest_Id;
    bool m_Completed;
};


class Quest_Test : public Quest
{
public:
    Quest_Test();

    bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature) override;
    bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id) override;
    void mt_Get_Description(std::vector<std::vector<sf::String>>& description) override;

private:
    int m_Phase;
};

class Quest_Main : public Quest
{
public:
    Quest_Main();

    bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature) override;
    bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id) override;
    void mt_Get_Description(std::vector<std::vector<sf::String>>& description) override;
};


class Quest_Troll : public Quest
{
public:
    Quest_Troll();

    bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature) override;
    bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id) override;
    void mt_Get_Description(std::vector<std::vector<sf::String>>& description) override;

    void mt_On_Leave(void);
    void mt_On_Fight(void);
    void mt_On_Pay(void);

    void mt_On_Victory(void);

private:
    bool m_Paid;
    bool m_Moving_Player;

    std::string m_Map_Music_Id;
};

class Quest_Dark_Mage : public Quest
{
public:
    Quest_Dark_Mage();


    bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature) override;
    bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id) override;
    void mt_Get_Description(std::vector<std::vector<sf::String>>& description) override;

    void mt_On_Victory(void);

    int m_Phase;
    bool m_Started;
    bool m_Troll_Asked;
    bool m_Troll_Help;
    bool m_Villager_Help;
    bool m_Villager_Seen;
    bool m_Mage_Seen;
};

class Quest_Tutoriel : public Quest
{
public:
    Quest_Tutoriel();

    bool mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature) override;
    bool mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id) override;
    void mt_Get_Description(std::vector<std::vector<sf::String>>& description) override;

    void mt_On_Victory(void);

    bool m_Wolf_Seen;
    bool m_Wolf_Defeated;
    bool m_Chest_Seen;
    bool m_Chest_Open;
};

#endif // _QUEST_HPP
