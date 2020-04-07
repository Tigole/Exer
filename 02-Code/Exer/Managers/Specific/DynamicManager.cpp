#include "DynamicManager.hpp"
#include "../../Context.hpp"
#include "../../_Misc.hpp"

#include <fstream>
#include <sstream>

DynamicManager::DynamicManager() : ResourceManager<Dynamic>("Dynamics")
{}

bool DynamicManager::mt_Load_Resource(const std::string& reference_file, std::unique_ptr<Dynamic>& resource)
{
    bool l_b_Ret;
    std::stringstream l_Stream;
    std::string l_File_Ext = reference_file.substr(reference_file.find_last_of('.') + 1);
    jaja::fp::File l_File;

    if (Context::smt_Get().m_Packer.mt_Get_File(reference_file, l_File))
    {
        l_Stream.str(std::string(l_File.m_Buffer, l_File.m_Size));
        if (l_File_Ext == "script")
        {
            std::string l_Line;
            Dynamic_Script* l_Script = new Dynamic_Script;

            while(fn_Get_Line(l_Stream, l_Line))
            {
                std::stringstream l_ss(l_Line);

                std::getline(l_ss, l_Line, ' ');

                if (l_Line == "DIM")
                {
                    l_ss >> l_Script->m_Dim.x >> l_Script->m_Dim.y;
                }
                else if (l_Line == "NAME")
                {
                    std::getline(l_ss, l_Script->m_Name, '\n');
                }
            }

            resource.reset(l_Script);
            l_b_Ret = true;
        }
        else if (l_File_Ext == "creature")
        {
            Creature* l_Creature = new Creature;
            std::string l_Line;
            std::string l_Sprite_Id;

            l_Creature->m_Gameplay_Data.m_Health = 1;
            l_Creature->m_Gameplay_Data.m_Health_Max = 1;

            l_Creature->m_Speed = 3.0f;

            while(fn_Get_Line(l_Stream, l_Line))
            {
                std::stringstream l_ss(l_Line);

                std::getline(l_ss, l_Line, ' ');

                if (l_Line == "SKILL")
                {
                    std::string l_Skill_Id;

                    l_ss >> l_Skill_Id;

                    l_Creature->m_Gameplay_Data.m_Skills.push_back(Context::smt_Get().m_Skills.mt_Get_Resource(l_Skill_Id));
                }
                else if (l_Line == "NAME")
                {
                    std::getline(l_ss, l_Creature->m_Name);
                }
                else if (l_Line == "SPRITE")
                {
                    std::string l_Anim_Id;
                    l_ss >> l_Anim_Id;

                    l_Creature->mt_Set_Sprite(l_Anim_Id);
                }
                else if (l_Line == "FRIEND")
                {
                    l_ss >> l_Creature->m_Friendly;
                }
                else if (l_Line == "MAP_SOLID")
                {
                    l_ss >> l_Creature->m_Solid_Map;
                }
                else if (l_Line == "DYN_SOLID")
                {
                    l_ss >> l_Creature->m_Solid_Dyn;
                }
                else if (l_Line == "DIAL_COLOR")
                {
                    l_ss >> l_Creature->m_Dialog_Color;
                }
                else if (l_Line == "HEALTH")
                {
                    l_ss >> l_Creature->m_Gameplay_Data.m_Health;
                }
                else if (l_Line == "HEALTH_MAX")
                {
                    l_ss >> l_Creature->m_Gameplay_Data.m_Health_Max;
                }
                else if (l_Line == "PSY")
                {
                    l_ss >> l_Creature->m_Gameplay_Data.m_Data_Aventure.m_Psy;
                }
                else if (l_Line == "PSY_MAX")
                {
                    l_ss >> l_Creature->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max;
                }
                else if (l_Line == "MOVEMENT")
                {
                    l_ss >> l_Creature->m_Gameplay_Data.m_Movement;
                }
                else if (l_Line == "AI_MOVEMENT_RAND")
                {
                    Creature_Control_AI_Rand* l_AI(new Creature_Control_AI_Rand(l_Creature));

                    l_ss >> l_Creature->m_Speed
                         >> l_AI->m_Threshold_Idle_To_Move_Min_ms >> l_AI->m_Threshold_Idle_To_Move_Max_ms
                         >> l_AI->m_Threshold_Move_To_Idle_Min_ms >> l_AI->m_Threshold_Move_To_Idle_Max_ms;

                    l_Creature->m_AI.reset(l_AI);

                }
                else if (l_Line == "AI_MOVEMENT_SQUARE")
                {
                    Creature_Control_AI_Square* l_AI(new Creature_Control_AI_Square(l_Creature));

                    l_ss >> l_Creature->m_Speed
                         >> l_AI->m_Threshold_Idle_To_Move_Min_ms >> l_AI->m_Threshold_Idle_To_Move_Max_ms
                         >> l_AI->m_Threshold_Move_To_Idle_Min_ms >> l_AI->m_Threshold_Move_To_Idle_Max_ms
                         >> l_AI->m_Square.left >> l_AI->m_Square.width >> l_AI->m_Square.top >> l_AI->m_Square.height;

                    l_Creature->m_AI.reset(l_AI);
                }
                else if (l_Line == "DIALOG")
                {
                    sf::String l_String;

                    fn_Get_Line(l_ss, l_Line);

                    for (std::size_t ii = 0; ii < l_Line.size(); ii++)
                    {
                        if (l_Line[ii] == '|')
                        {
                            l_Creature->m_Dialog.push_back(l_String);
                            l_String.clear();
                        }
                        else if (l_Line[ii] == '\\' && ((l_Line[ii+1] == 'n') || (l_Line[ii+1] == 'r')))
                        {
                            l_String += '\n';
                            ii++;
                        }
                        else
                        {
                            l_String += l_Line[ii];
                        }
                    }

                    l_Creature->m_Dialog.push_back(l_String);
                }
                else
                {
                    l_ss.seekg(0, std::ios_base::beg);
                    l_ss >> l_Creature->m_Name >> l_Sprite_Id >> l_Creature->m_Friendly >> l_Creature->m_Solid_Map >> l_Creature->m_Solid_Dyn;

                    l_Creature->m_Gameplay_Data.m_Health = 10;
                    l_Creature->m_Gameplay_Data.m_Health_Max = 10;
                    l_Creature->m_Gameplay_Data.m_Data_Aventure.m_Psy = 10;
                    l_Creature->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max = 10;

                    l_Creature->mt_Set_Sprite(l_Sprite_Id);
                }
            }
            resource.reset(l_Creature);
            l_b_Ret = true;
        }
        else if (l_File_Ext == "chest")
        {
            Dynamic_Chest* l_Chest = new Dynamic_Chest;
            std::string l_Line;

            while(fn_Get_Line(l_Stream, l_Line))
            {
                std::stringstream l_ss(l_Line);

                std::getline(l_ss, l_Line, ' ');

                if (l_Line == "ITEM")
                {
                    ChestData l_Data;
                    std::getline(l_ss, l_Data.m_Item_Id, ' ');

                    l_ss >> l_Data.m_Quantity >> (int&)l_Data.m_Item_Type;

                    l_Chest->m_Items.push_back(l_Data);
                }
                else if (l_Line == "ANIM")
                {
                    std::getline(l_ss, l_Line, ' ');

                    l_Chest->m_Sprite = Context::smt_Get().m_SpriteSheetAnimators.mt_Get_Resource(l_Line);
                }
            }

            resource.reset(l_Chest);
            l_b_Ret = true;
        }
        else if (l_File_Ext == "light")
        {
            Dynamic_Light* l_Light = new Dynamic_Light;
            std::string l_Line;

            while(fn_Get_Line(l_Stream, l_Line))
            {
                std::stringstream l_ss(l_Line);

                std::getline(l_ss, l_Line, ' ');

                if (l_Line == "DIM")
                {
                    l_ss >> l_Light->m_Dim.x >> l_Light->m_Dim.y;
                }
                else if (l_Line == "COLOR")
                {
                    int l_tmp;
                    l_ss >> l_tmp;
                    l_Light->m_Final_Color.r = l_tmp;
                    l_ss >> l_tmp;
                    l_Light->m_Final_Color.g = l_tmp;
                    l_ss >> l_tmp;
                    l_Light->m_Final_Color.b = l_tmp;
                }
                else if (l_Line == "NAME")
                {
                    std::getline(l_ss, l_Light->m_Name, '\n');
                }
                else if (l_Line == "DURATION")
                {
                    l_ss >> l_Light->m_Duration_s;
                }
            }

            resource.reset(l_Light);
            l_b_Ret = true;
        }
        else if (l_File_Ext == "music")
        {
            Dynamic_Music* l_Music = new Dynamic_Music;
            std::string l_Line;

            while(fn_Get_Line(l_Stream, l_Line))
            {
                std::stringstream l_ss(l_Line);

                std::getline(l_ss, l_Line, ' ');

                if (l_Line == "TRANS")
                {
                    l_ss >> l_Music->m_Transition_Time;
                }
                else if (l_Line == "MUSIC")
                {
                    std::getline(l_ss, l_Music->m_Music_Id, '\n');
                }
                else if (l_Line == "DIM")
                {
                    l_ss >> l_Music->m_Dim.x >> l_Music->m_Dim.y;
                }
            }

            resource.reset(l_Music);
            l_b_Ret = true;
        }
    }
    else
    {
        l_b_Ret = false;
    }

    return l_b_Ret;
}

void DynamicManager::mt_Unload_Resource(Dynamic* resource)
{
    Creature* l_Creature = dynamic_cast<Creature*>(resource);
    if (l_Creature != nullptr)
    {
        //Context::smt_Get().m_SpriteSheetAnimators.mt_Release(l_Creature->m_Sprite);
    }
}
