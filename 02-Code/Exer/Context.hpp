#ifndef _CONTEXT_HPP
#define _CONTEXT_HPP 1

#include "Managers/FontManager.hpp"
#include "Managers/MusicManager.hpp"
#include "Managers/SoundManager.hpp"
#include "Managers/TextureManager.hpp"
#include "Managers/ShaderManager.hpp"

#include "Managers/Specific/TilesetManager.hpp"
#include "Managers/Specific/SpriteSheetManager.hpp"
#include "Managers/Specific/MapManager.hpp"
#include "Managers/Specific/DynamicManager.hpp"
#include "Managers/Specific/QuestManager.hpp"
#include "Managers/Specific/ScriptManager.hpp"
#include "Managers/Specific/ItemManager.hpp"
#include "Managers/Specific/SkillManager.hpp"
#include "Managers/Specific/AnimationManager.hpp"

#include "Systems/SystemAnimation.hpp"
#include "Systems/SystemSound.hpp"

#include "Event.hpp"

#include "FilePacker/FilePacker.hpp"
#include "PathFinder.hpp"

class GameEngine;

struct Context
{
    static Context& smt_Get(void)
    {
        static Context ls_Context;
        return ls_Context;
    }

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
private:
    Context(){}
    ~Context(){}

public:
    FontManager m_Fonts;
    TextureManager m_Textures;
    MusicManager m_Musics;
    SoundManager m_Sounds;
    ShaderManager m_Shaders;

    TilesetManager m_Tilesets;
    SpriteSheetManager m_SpriteSheets;
    SpriteSheetAnimatorManager m_SpriteSheetAnimators;
    MapManager m_Maps;
    DynamicManager m_Dynamics;
    QuestManager m_Quests;
    ScriptManager m_Scripts;
    SkillManager m_Skills;
    ItemFactory m_Items;
    AnimationManager m_Animations;

    GameEngine* m_Engine;
    SystemDialog* m_Dialog;
    SystemAnimation* m_System_Animation;
    SystemSound m_System_Sound;

    PathFinder m_PathFinder;


    jaja::fp::FilePacker m_Packer;
};

#endif // _CONTEXT_HPP
