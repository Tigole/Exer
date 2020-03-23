#ifndef _STATE_HPP
#define _STATE_HPP 1

#include <iostream>

template<typename T>
struct State
{
    State& operator=(const T& new_state)
    {
        m_Previous = m_Showing_Previous = m_Current;
        m_Current = new_state;

        std::cout << "[State]: " << m_Showing_Previous << " -> " << m_Current << "\n";

        return *this;
    }
    bool operator==(const T& s)
    {
        return m_Current == s;
    }
    T m_Current;
    T m_Previous;

    bool mt_On_Entry(void)
    {
        return m_Current != m_Previous;
    }

    bool mt_On_Entry(const T& state)
    {
        return mt_On_Entry() && m_Current == state;
    }

private:
    T m_Showing_Previous;
};


#endif // _STATE_HPP
