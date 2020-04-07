#include "AnimationManager.hpp"
#include "../../Context.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

AnimationManager::AnimationManager()
 :  ResourceManager<Animation_Data>("AnimationData")
{}

bool AnimationManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Animation_Data>& resource)
{
    bool l_b_Ret;
    std::stringstream l_Stream;
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File))
    {
        Animation_Data* l_Data = new Animation_Data;
        std::string l_Line;

        l_Stream.str(l_File.mt_To_String());

        while(fn_Get_Line(l_Stream, l_Line))
        {
            std::stringstream l_ss(l_Line);

            std::getline(l_ss, l_Line, ' ');
            if (l_Line == "SPRITESHEET")
            {
                std::string l_SpriteSheet_Id;

                l_ss >> l_SpriteSheet_Id;

                l_Data->m_SpriteSheet = Context::smt_Get().m_SpriteSheets.mt_Get_Resource(l_SpriteSheet_Id);
            }
            else if (l_Line == "FRAME")
            {
                l_Data->m_Phases.push_back(sf::Vector2<std::size_t>(0, 0));
                l_ss >> l_Data->m_Phases.back().x >> l_Data->m_Phases.back().y;
            }
            else if (l_Line == "SOUND")
            {
                std::size_t l_Index;
                std::string l_Sound_Id;

                std::getline(l_ss, l_Sound_Id, ' ');
                l_ss >> l_Index;

                l_Data->m_Sounds.push_back(std::make_pair(l_Index, l_Sound_Id));
            }
        }

        resource.reset(l_Data);

        l_b_Ret = true;
    }
    else
    {
        l_b_Ret = false;
    }

    return true;
}

void AnimationManager::mt_Unload_Resource(Animation_Data* resource)
{
    Context::smt_Get().m_SpriteSheets.mt_Release(resource->m_SpriteSheet);
}
