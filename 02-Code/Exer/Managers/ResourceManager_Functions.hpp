#ifndef _RESOURCE_MANAGER_FUNCTIONS_HPP
#define _RESOURCE_MANAGER_FUNCTIONS_HPP

#include "../Context.hpp"
#include "ResourceManager.hpp"

template<class C>
bool fn_Load_Config(ResourceManager<C>& mngr, const std::string& file_desc)
{
    bool l_b_Ret(false);
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(file_desc, l_File))
    {
        mngr.mt_Load_CFG(l_File);
        l_b_Ret = true;
    }

    return l_b_Ret;
}

#endif // _RESOURCE_MANAGER_FUNCTIONS_HPP
