#include "PathFinder.hpp"

#include "Context.hpp"
#include "GameEngine.hpp"


PathFinder::PathFinder() : m_Thread(&PathFinder::mt_Thread_Find, this), m_State(PathFinderState::Done)
{}

float PathFinder::mt_Distance(const Node* src, const Node* tgt) const
{
    float x, y;

    x = src->m_Pos.x - tgt->m_Pos.x;
    y = src->m_Pos.y - tgt->m_Pos.y;

    return sqrt(x*x + y*y);
}

float PathFinder::mt_Heuristic(const Node* src, const Node* tgt) const
{
    float l_Ret(INFINITY);

    if (!m_Map->mt_Is_Solid(src->m_Pos.x, src->m_Pos.y) && !m_Map->mt_Is_Solid(tgt->m_Pos.x, tgt->m_Pos.y))
    {
        sf::FloatRect l_Hitbox;
        Dynamic* l_Dyn;

        l_Ret = mt_Distance(src, tgt);

        for(std::size_t ii = 0; ii < m_Dyns->size(); ii++)
        {
            l_Dyn = (*m_Dyns)[ii].m_Resource;

            l_Hitbox.left = l_Dyn->m_Pos.x;
            l_Hitbox.top = l_Dyn->m_Pos.y;
            l_Hitbox.width = l_Dyn->m_Dim.x;
            l_Hitbox.height = l_Dyn->m_Dim.y;

            if (l_Hitbox.contains(src->m_Pos))
            {
                l_Ret *= 2.0f;
            }
            if (l_Hitbox.contains(tgt->m_Pos))
            {
                l_Ret *= 2.0f;
            }
        }
    }

    return l_Ret;
}

void PathFinder::mt_Find(const Map& map, const Creature* src, const Creature* tgt)
{
    m_Src = src;
    m_Tgt = tgt;
    m_Map = &map;

    if (m_State == PathFinderState::Searching)
    {
        std::cout << "mt_Find\n";
        m_Stop_Required = true;
        m_Thread.wait();
    }

    m_Found = false;
    m_Stop_Required = false;
    m_State = PathFinderState::Searching;
    m_Thread.launch();

    //mt_Find_Old_Way();
}

void PathFinder::mt_Thread_Find(void)
{
    mt_Find_Old_Way();
    std::cout << "mt_Thread_Find\n";
}

void PathFinder::mt_Find_Old_Way(void)
{
    bool l_b_Ret(false);
    Node* l_Map_Nodes;
    std::list<Node*> l_Open_Nodes;
    Node* l_Current_Node;
    Node* l_End_Node;
    float l_Local_Goal;
    Node* l_Neighbour;
    Dynamic* l_Dyn;

    m_State = PathFinderState::Searching;
    fn_Clear_Stack(m_Path);

    l_Map_Nodes = new Node[m_Map->m_Width * m_Map->m_Height];
    for (std::size_t yy = 0; yy < m_Map->m_Height; yy++)
    {
        for (std::size_t xx = 0; xx < m_Map->m_Width; xx++)
        {
            std::size_t ii = xx + yy * m_Map->m_Width;
            std::size_t north = xx + (yy - 1) * m_Map->m_Width;
            std::size_t est = xx + 1 + yy * m_Map->m_Width;
            std::size_t south = xx + (yy + 1) * m_Map->m_Width;
            std::size_t west = xx - 1+ yy * m_Map->m_Width;

            if (xx > 0)
            {
                l_Map_Nodes[ii].m_Neighbours.push_back(&l_Map_Nodes[west]);
            }
            if (xx <m_Map->m_Width)
            {
                l_Map_Nodes[ii].m_Neighbours.push_back(&l_Map_Nodes[est]);
            }
            if (yy > 0)
            {
                l_Map_Nodes[ii].m_Neighbours.push_back(&l_Map_Nodes[north]);
            }
            if (yy < m_Map->m_Height)
            {
                l_Map_Nodes[ii].m_Neighbours.push_back(&l_Map_Nodes[south]);
            }

            l_Map_Nodes[ii].m_Global_Goal = INFINITY;
            l_Map_Nodes[ii].m_Local_Goal = INFINITY;
            l_Map_Nodes[ii].m_Pos = sf::Vector2f(xx, yy);
            l_Map_Nodes[ii].m_Visited = false;
            l_Map_Nodes[ii].m_Parent = nullptr;
        }
    }

    l_End_Node = &l_Map_Nodes[static_cast<int>(m_Tgt->m_Pos.x + 0.5f) + static_cast<int>(m_Tgt->m_Pos.y + 0.5f) * m_Map->m_Width];

    l_Current_Node = &l_Map_Nodes[static_cast<int>(m_Src->m_Pos.x + 0.5f) + static_cast<int>(m_Src->m_Pos.y + 0.5f) * m_Map->m_Width];
    l_Current_Node->m_Global_Goal = mt_Heuristic(l_Current_Node, l_End_Node);
    l_Current_Node->m_Local_Goal = 0.0f;

    l_Open_Nodes.push_back(l_Current_Node);

    //std::cout << "Player pos: " << Context::smt_Get().m_Engine->m_Player->m_Pos << '\n';

    while(!l_Open_Nodes.empty() && (l_Current_Node != l_End_Node) && (m_Stop_Required == false))
    {
        /// 1. Sort by best possible solution
        l_Open_Nodes.sort([](const Node* a, const Node* b){return a->m_Global_Goal < b->m_Global_Goal;});

        /// 2. remove all best solution already visited
        while(!l_Open_Nodes.empty() && l_Open_Nodes.front()->m_Visited)
        {
            l_Open_Nodes.pop_front();
        }

        /// 3. Algorithm
        if (!l_Open_Nodes.empty())
        {
            l_Current_Node = l_Open_Nodes.front();
            l_Current_Node->m_Visited = true;

            //std::cout << "Current: " << l_Current_Node->m_Pos << " : " << l_Current_Node->m_Global_Goal << '\n';

            /// 4. For each neighbour,
            for (auto& n : l_Current_Node->m_Neighbours)
            {
                l_Neighbour = n;
                /// 4.1. Add it to the list if not visited
                if (l_Neighbour->m_Visited == false)
                {
                    l_Open_Nodes.push_back(l_Neighbour);
                }

                /// 4.2. Check local goal
                l_Local_Goal = l_Current_Node->m_Local_Goal + mt_Heuristic(l_Current_Node, l_Neighbour);

                if (l_Local_Goal < l_Neighbour->m_Local_Goal)
                {
                    l_Neighbour->m_Local_Goal = l_Local_Goal;
                    l_Neighbour->m_Parent = l_Current_Node;

                    l_Neighbour->m_Global_Goal = l_Neighbour->m_Local_Goal + mt_Heuristic(l_Neighbour, l_End_Node);

                    //std::cout << l_Neighbour->m_Pos << " : " << l_Neighbour->m_Global_Goal << '\n';
                }
            }
        }
    }

    //std::cout << "Final: " << l_Current_Node->m_Pos << " : " << l_Current_Node->m_Global_Goal << '\n';

    while(l_Current_Node != nullptr)
    {
        m_Path.push(l_Current_Node->m_Pos);
        l_Current_Node = l_Current_Node->m_Parent;
    }

    m_Found = (l_End_Node->m_Parent != nullptr);

    delete[]l_Map_Nodes;

    m_State = PathFinderState::Done;
}

bool PathFinder::mt_Path_Found(std::stack<sf::Vector2f>& path) const
{
    if (m_Found == true)
    {
        path = m_Path;
    }

    return m_Found;
}

bool PathFinder::mt_Is_Searching(void) const
{
    return (m_State == PathFinderState::Searching);
}
