#ifndef _PATH_FINDER_HPP
#define _PATH_FINDER_HPP 1

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Thread.hpp>
#include <vector>
#include <stack>
#include "Managers/ResourceManager.hpp"

class Map;
class Dynamic;
class Creature;

struct Node
{
    bool m_Visited;
    float m_Global_Goal;
    float m_Local_Goal;
    sf::Vector2f m_Pos;
    std::vector<Node*> m_Neighbours;
    Node* m_Parent;
};


enum class PathFinderState
{
    Searching,
    Done,
};

class PathFinder
{
public:
    PathFinder();

public:
    void mt_Find(const Map& map, const Creature* src, const Creature* tgt);
    std::vector<Resource<Dynamic>>* m_Dyns;
protected:
    const Map* m_Map;
    const Creature* m_Src;
    const Creature* m_Tgt;
    std::stack<sf::Vector2f> m_Path;

    void mt_Thread_Find(void);
    sf::Thread m_Thread;

    void mt_Find_Old_Way(void);

    float mt_Distance(const Node* src, const Node* tgt) const;
    float mt_Heuristic(const Node* src, const Node* tgt) const;


public:
    bool mt_Path_Found(std::stack<sf::Vector2f>& path) const;
    bool mt_Is_Searching(void) const;

protected:
    PathFinderState m_State;
    bool m_Stop_Required;
    bool m_Found;
};

#endif // _PATH_FINDER_HPP
