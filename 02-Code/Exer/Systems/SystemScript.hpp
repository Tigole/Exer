#ifndef _SYSTEM_SCRIPT_HPP
#define _SYSTEM_SCRIPT_HPP 1

#include <list>
#include <memory>
#include "../Command.hpp"

class SystemScript
{
public:
    SystemScript();

    void mt_Add_Command(Command* c);
    void mt_Process_Command(float elapsed_time);
    void mt_Complete_Current_Command(void);

    bool m_User_Ctrl;
    bool m_Light_Ctrl;
    bool m_Check_Dyn_Collision;

private:
    std::list<std::unique_ptr<Command>> m_Commands;

};

#endif // _SYSTEM_SCRIPT_HPP
