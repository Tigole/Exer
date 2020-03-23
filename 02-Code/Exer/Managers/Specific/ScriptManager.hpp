#ifndef _SCRIPT_MANAGER_HPP
#define _SCRIPT_MANAGER_HPP 1

#include "../ResourceManager.hpp"
#include "../../Command.hpp"

class ScriptManager : public ResourceManager<Command>
{
public:
    ScriptManager();
protected:
    bool mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Command>& resource) override;
    void mt_Unload_Resource(Command* resource) override;
};

#endif // _SCRIPT_MANAGER_HPP
