#include "GameEngine.hpp"
#include <iostream>
#include "Managers/ResourceManager_Functions.hpp"


std::ostream& operator<<(std::ostream& o, GameStateType gs)
{
    switch(gs)
    {
    case GameStateType::Display_Inventory:
        o << "Inventory"; break;
    case GameStateType::Display_Quests:
        o << "Quests"; break;
    case GameStateType::Fight:
        o << "Fight"; break;
    case GameStateType::Game:
        o << "Game"; break;
    case GameStateType::Display_Skills:
        o << "Skills"; break;
    case GameStateType::Game_Over:
        o << "Game_Over"; break;
    case GameStateType::EndGame:
        o << "EndGame"; break;
    case GameStateType::Close:
        o << "Close"; break;
    case GameStateType::Choose_Class:
        o << "Choose_Class"; break;
    default:
        o << "???"; break;
    }
    return o;
}



void fn_Main(void)
{
    GameEngine l_Game;

    if (!Context::smt_Get().m_Packer.mt_Parse("Assets.jaja"))
    {
        std::cout << "Failed parsing resources\n";
        return;
    }

    fn_Load_Config(Context::smt_Get().m_Fonts, "Assets/Fonts.cfg");
    fn_Load_Config(Context::smt_Get().m_Textures, "Assets/Textures.cfg");
    Context::smt_Get().m_Sounds.mt_Load("Assets/Sounds.cfg");
    fn_Load_Config(Context::smt_Get().m_Musics, "Assets/Musics.cfg");
    fn_Load_Config(Context::smt_Get().m_Shaders, "Assets/Shaders.cfg");
    fn_Load_Config(Context::smt_Get().m_Tilesets, "Assets/Tilesets.cfg");
    fn_Load_Config(Context::smt_Get().m_SpriteSheets, "Assets/SpriteSheets.cfg");
    fn_Load_Config(Context::smt_Get().m_SpriteSheetAnimators, "Assets/TilesetAnims.cfg");
    fn_Load_Config(Context::smt_Get().m_Maps, "Assets/Maps.cfg");
    fn_Load_Config(Context::smt_Get().m_Dynamics, "Assets/Dynamics.cfg");
    fn_Load_Config(Context::smt_Get().m_Quests, "Assets/Quests.cfg");
    //Context::smt_Get().m_Scripts.mt_Load("");
    Context::smt_Get().m_Items.mt_Load("Assets/Items.cfg");
    fn_Load_Config(Context::smt_Get().m_Skills, "Assets/Skills.cfg");
    fn_Load_Config(Context::smt_Get().m_Animations, "Assets/Animations.cfg");
    Context::smt_Get().m_PathFinder.m_Dyns = &l_Game.m_Dyn;
    Context::smt_Get().m_System_Animation = &l_Game.m_Animation;

    sf::Listener::setGlobalVolume(20.0f);

    try
    {
        l_Game.mt_Create();

        l_Game.mt_Run();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}



void GameEngine::mt_Create(void)
{
    sf::Vector2i l_Tile_Count(32, 22);
    Context::smt_Get().m_Engine = this;

    m_FPS = 60;
    m_Wnd.create(sf::VideoMode(l_Tile_Count.x * 32, l_Tile_Count.y * 32), "Exer", sf::Style::Titlebar | sf::Style::Close);
    m_Wnd.setMouseCursorVisible(false);

    m_Camera.mt_Initialize(m_Wnd);

    m_State.m_Current = GameStateType::Game;
    m_State.m_Previous = GameStateType::Game;

    m_Quest_State.mt_Create();
    m_Game_State.mt_Create();
    m_Skill_State.mt_Create();
    m_Inventory_State.mt_Create();
    m_Creation_State.mt_Create();
    m_EndGame_State.mt_Create();

    m_Sky_Color = sf::Color::White;
    m_Sky_Color_Accumulated_Time_s = 0.0f;

    mt_New_Game("");

    std::cout << m_Wnd.getSettings().majorVersion << ' ' << m_Wnd.getSettings().minorVersion << '\n';
}

void GameEngine::mt_Handle_Event(sf::Event& event)
{
    if (event.type == sf::Event::Resized)
    {
        sf::FloatRect l_New_View;

        l_New_View.width = event.size.width;
        l_New_View.height = event.size.height;
//        l_New_View.left = m_Camera_View.getCenter().x - m_Camera_View.getSize().x / 2;
//        l_New_View.top = m_Camera_View.getCenter().y - m_Camera_View.getSize().y / 2;

//        m_Camera_View.reset(l_New_View);
//        m_Wnd.setView(m_Camera_View);
    }
    else
    {
        switch(m_State.m_Previous)
        {
        case GameStateType::Game:
            m_Game_State.mt_Handle_Event(event);
            break;
        case GameStateType::Display_Quests:
            m_Quest_State.mt_Handle_Event(event);
            break;
        case GameStateType::Display_Inventory:
            m_Inventory_State.mt_Handle_Event(event);
            break;
        case GameStateType::Fight:
            m_Fight_State.mt_Handle_Event(event);
            break;
        case GameStateType::Display_Skills:
            m_Skill_State.mt_Handle_Event(event);
            break;
        case GameStateType::Choose_Class:
            m_Creation_State.mt_Handle_Event(event);
            break;
        case GameStateType::EndGame:
            m_EndGame_State.mt_Handle_Event(event);
            break;
        case GameStateType::Close:
            break;
        case GameStateType::Game_Over:
            Context::smt_Get().m_Dialog->mt_Handle_Event(event);
            if (Context::smt_Get().m_Dialog->mt_Has_Event_Priority() == false)
            {
                if (event.type == sf::Event::KeyReleased)
                {
                    if ((event.key.code == sf::Keyboard::Space) || (event.key.code == sf::Keyboard::Escape))
                    {
                        m_State = GameStateType::Close;
                    }
                }
            }
            break;
        case GameStateType::COUNT:
        default:
            m_State = GameStateType::Game;
        }
    }
}

void GameEngine::mt_Update(float delta_time_s)
{
    /*if (m_Script.m_Light_Ctrl == false)
    {
        m_Sky_Color_Accumulated_Time_s += delta_time_s;

        if (m_Sky_Color_Accumulated_Time_s > 30.0f)
        {
            m_Sky_Color_Accumulated_Time_s = -10.0f;
        }
        else if (m_Sky_Color_Accumulated_Time_s > 20.0f)
        {
            m_Script.mt_Add_Command(new Command_Lights(m_Sky_Color, sf::Color::White, 3.0f));
        }
        else if (m_Sky_Color_Accumulated_Time_s > 10.0f)
        {
            m_Script.mt_Add_Command(new Command_Lights(m_Sky_Color, sf::Color(218, 165, 32), 3.0f));
        }
        else if (m_Sky_Color_Accumulated_Time_s > 0.0f)
        {
            m_Script.mt_Add_Command(new Command_Lights(m_Sky_Color, sf::Color(128, 128, 255), 3.0f));
        }
    }*/

    switch(m_State.m_Previous)
    {
    case GameStateType::Game:
        m_Game_State.mt_Update(delta_time_s);
        break;
    case GameStateType::Display_Quests:
        m_Quest_State.mt_Update(delta_time_s);
        break;
    case GameStateType::Display_Inventory:
        m_Inventory_State.mt_Update(delta_time_s);
        break;
    case GameStateType::Fight:
        m_Fight_State.mt_Update(delta_time_s);
        break;
    case GameStateType::Display_Skills:
        m_Skill_State.mt_Update(delta_time_s);
        break;
    case GameStateType::Choose_Class:
        m_Creation_State.mt_Update(delta_time_s);
        break;
    case GameStateType::EndGame:
        m_EndGame_State.mt_Update(delta_time_s);
        break;
    case GameStateType::Close:
        m_b_Close_Window = true;
        break;
    case GameStateType::Game_Over:
    {
        m_Script.mt_Process_Command(delta_time_s);
        Context::smt_Get().m_Dialog->mt_Update(delta_time_s);
        break;
    }
    case GameStateType::COUNT:
    default:
        m_State = GameStateType::Game;
    }

    m_Camera.mt_Update(delta_time_s);
}

void GameEngine::mt_Draw(sf::RenderTarget& target)
{
    switch(m_State.m_Previous)
    {
    case GameStateType::Game:
        m_Game_State.mt_Draw(target);
        break;
    case GameStateType::Display_Quests:
        m_Quest_State.mt_Draw(target);
        break;
    case GameStateType::Display_Inventory:
        m_Inventory_State.mt_Draw(target);
        break;
    case GameStateType::Fight:
        m_Fight_State.mt_Draw(target);
        break;
    case GameStateType::Display_Skills:
        m_Skill_State.mt_Draw(target);
        break;
    case GameStateType::Choose_Class:
        m_Creation_State.mt_Draw(target);
        break;
    case GameStateType::EndGame:
        m_EndGame_State.mt_Draw(target);
        break;
    case GameStateType::Close:
        break;
    case GameStateType::Game_Over:
    {
        m_Game_State.mt_Draw(target);
        break;
    }
    case GameStateType::COUNT:
    default:
        m_State = GameStateType::Game;
    }

    if (m_State.mt_On_Entry())
    {
        switch(m_State.m_Current)
        {
        case GameStateType::Game:
            if (m_State.m_Previous == GameStateType::Choose_Class)
            {
                m_Script.mt_Add_Command(new Command_Music("", 3.0f));
                m_Script.mt_Add_Command(new Command_Camera(m_Map->m_Tileset->mt_Cell_To_Pixel(m_Player->m_Pos), 0.2f, new Interpolator_Linear, false));
                m_Script.mt_Add_Command(new Command_ShowDialog({fn_Dialog(m_Player, "Ça fait plusieurs jours que je suis dans cette grotte..."),
                                                               fn_Dialog(m_Player, "Mais il me semble que depuis quelques mètres l'odeur a changé."),
                                                               fn_Dialog(m_Player, "La sortie ?"),
                                                               fn_Dialog(m_Player, "Oui !"),
                                                               fn_Dialog(m_Player, "Enfin !")}));
                m_Script.mt_Add_Command(new Command_Camera(m_Map->m_Tileset->mt_Cell_To_Pixel(m_Player->m_Pos + sf::Vector2f(20, 15)), 2.0f, new Interpolator_Cos, false));
                //m_Script.mt_Add_Command(new Command_Camera(m_Map->m_Tileset->mt_Cell_To_Pixel(m_Player->m_Pos), 1.0f, new Interpolator_Cos));
                m_Script.mt_Add_Command(new Command_Music("Map", 1.0f));
                m_Script.mt_Add_Command(new Command_Lights(sf::Color::Black, sf::Color::White, 2.0f));
                m_Script.mt_Add_Command(new Command_ShowDialog({fn_Dialog(m_Player, "Ah !"),
                                                               fn_Dialog(m_Player, "L'air frais !")}));
                m_Script.mt_Add_Command(new Command_Sound("Wind", sf::Vector3f(0.0f, 0.0f, 0.0f), true));
                m_Script.mt_Add_Command(new Command_ShowDialog({fn_Dialog(m_Player, "Aller !"),
                                                               fn_Dialog(m_Player, "Descendons  le plus vite possible de cette maudite passe !"),
                                                               fn_Dialog(m_Player, "Retrouvons la civilisation !")}));


                m_System_Quest.mt_Add_Quest("Main_Quest");
                m_System_Quest.mt_Add_Quest("Tutoriel");
            }
            break;
        case GameStateType::EndGame:
            m_Script.mt_Add_Command(new Command_Music("Fin", 0.5f));
            m_Script.mt_Add_Command(new Command_Wait(1.2f));
            m_Script.mt_Add_Command(new Command_ShowDialog({"Merci milles fois d'avoir joué\net bravo d'être arrivé jusqu'au bout !",
                                                           "J'espère que tu as passé\nun bon moment sur ce jeu !",
                                                           "N'hésite pas à rejouer pour changer de classe\net essayer d'autres options de dialogue !",
                                                           "Encore une fois merci !\nEt à bientôt pour " + fn_Command_Key("Lithomancia") + " !",
                                                           "JaJa"}));
            m_Script.mt_Add_Command(new Command_Music("", 2.0f));
            m_Script.mt_Add_Command(new Command_End_Game());
            break;
        case GameStateType::Game_Over:
            m_Script.mt_Add_Command(new Command_Lights(m_Sky_Color, sf::Color::Black, 2.0f));
            m_Script.mt_Add_Command(new Command_ShowDialog({"Game Over",
                                                            "Il ne faut sutout pas rester sur une défaite !",
                                                            "Peut-être que vous pouvez faire mieux ?"}));
            break;
        }

        std::cout << "GAME STATE: " << m_State.m_Previous << " -> " << m_State.m_Current << '\n';
        m_State.m_Previous = m_State.m_Current;
    }
}

void GameEngine::mt_On_Exit(void)
{
    if (m_Music != nullptr)
    {
        m_Music->stop();
        Context::smt_Get().m_Musics.mt_Release(m_Music);
        m_Inventory_State.mt_Destroy();
        m_Skill_State.mt_Destroy();
        m_Game_State.mt_Destroy();
        m_Quest_State.mt_Destroy();
        m_Creation_State.mt_Destroy();
        m_EndGame_State.mt_Destroy();
    }
}

Creature* GameEngine::mt_Get_Creature(const std::string& creature_id)
{
    Creature* l_Ret(nullptr);

    if (creature_id == "player")
    {
        l_Ret = m_Player;
    }
    else
    {
        auto l_it = std::find_if(m_Dyn.begin(), m_Dyn.end(), [&](const Resource<Dynamic> d){return d->m_Name == creature_id;});
        if (l_it != m_Dyn.end())
        {
            l_Ret = dynamic_cast<Creature*>(l_it->m_Resource);
        }
    }

    return l_Ret;
}



void GameEngine::mt_Start_Fight(const std::map<int, std::vector<CommandFightCreature>>& fighters, std::function<void(void)> pfn_On_Victory, const std::string& music_id)
{
    m_Fight_State.m_Fight.mt_Start_Fight(fighters, pfn_On_Victory, mt_Get_Music_Id(), music_id);
    m_State = GameStateType::Fight;
}


void GameEngine::mt_Change_Map(const std::string& map_id, const sf::Vector2f& player_pos)
{
    m_Dyn.resize(1);

    if (m_Map != nullptr)
        Context::smt_Get().m_Maps.mt_Release(m_Map);

    m_Map = Context::smt_Get().m_Maps.mt_Get_Resource(map_id);

    m_Dyn[0]->m_Pos = player_pos;

    m_Map->mt_Populate_Dynamics(m_Dyn);

    for (auto& q : m_System_Quest.m_Quests)
        q->mt_Populate_Dynamics(m_Dyn, m_Map->m_Name);

    m_Player = dynamic_cast<Creature*>(m_Dyn[0].m_Resource);
    m_Camera.mt_Set_Tgt(m_Player);
}

void GameEngine::mt_New_Game(const std::string& file)
{
    sf::Vector2f l_Player_Pos(18.0f, 19.0f);
    bool l_Dbg(false);
    Creature* l_Troll = nullptr;

    m_Dyn.push_back(Context::smt_Get().m_Dynamics.mt_Get_Resource("Player"));

    if (l_Dbg == true)
    {
        /// Troll
        //l_Player_Pos = {31.0f, 61.0f};

        /// Apres troll
        l_Player_Pos = {81.0f, 58.0f};

        /// Bob
        //l_Player_Pos = {97.0f, 101.0f};

        /// Mage
        //l_Player_Pos = {76.0f, 20.0f};
    }

    /// Fin
    //l_Player_Pos = {144.0f, 50.0f};

    mt_Change_Map("Road", l_Player_Pos);

    if (l_Dbg)
    {
        m_Inventory.mt_Change_Item_Count("LifePotion", 20, ItemType::Edible);
        m_Inventory.mt_Change_Item_Count("PsyPotion", 20, ItemType::Edible);
        m_Inventory.mt_Change_Item_Count("Medicinal_Herb", 1, ItemType::Quest);

        m_Sky_Color = sf::Color::White;

        /// Sombres_Manigances - Troll
        Creature* l_Villageois = mt_Get_Creature("Arthur");
        l_Troll = mt_Get_Creature("Troll");

        Quest_Dark_Mage* l_Quest = dynamic_cast<Quest_Dark_Mage*>(m_System_Quest.mt_Add_Quest("Sombres_Manigances"));

        l_Quest->m_Mage_Defeated_Before_Herbe = false;

        //l_Troll->m_Pos = sf::Vector2f(69.0f, 21.0f);
        //l_Quest->m_Troll_Help = true;
        //l_Villageois->m_Pos = sf::Vector2f(69.0f, 19.0f);
        //l_Quest->m_Villager_Help = true;
    }
    else
    {
        l_Troll = mt_Get_Creature("Troll");
        l_Troll->mt_LookDir(sf::Vector2f(-1.0f, 0.0f));
        l_Troll->m_Pos.y -= 0.5f;
        m_Sky_Color = sf::Color::Black;
        m_Script.mt_Add_Command(new Command_Lights(sf::Color::Black, sf::Color::Black, 0.02f));
        m_Script.mt_Add_Command(new Command_Wait(1.0f));
        m_Script.mt_Add_Command(new Command_ShowDialog({"_Contrôles du jeu :_ \nValide tes actions avec #F4661B *Espace* #white \nAnnule avec #F4661B *Echap* #white \nDéplace toi avec les #F4661B *flèches directionnelles* #white"}));
        m_Script.mt_Add_Command(new Command_Music("Presentation", 2.0f));
        m_Script.mt_Add_Command(new Command_Wait(2.0f));
        m_Script.mt_Add_Command(new Command_ShowDialog({"Bienvenue dans " + fn_Command_Key("Exer", sf::Color::White) + ".",
                                                       "C'est après 10 mois de développement\net près de 30 000 lignes de code,\nque je suis fier de présenter ce 1er RPG.",
                                                       "Mais assez passé de temps en présentations,\nà toi de jouer !",
                                                       "Bonne aventure !",
                                                       "JaJa"}));
        m_Script.mt_Add_Command(new Command_Wait(1.0f));
        m_Script.mt_Add_Command(new Command_Set_Intro());
        /*m_Script.mt_Add_Command(new Command_Music("Introduction", 3.0f));
        m_Script.mt_Add_Command(new Command_Camera(m_Map->m_Tileset->mt_Cell_To_Pixel(m_Player->m_Pos), 0.2f, new Interpolator_Linear, false));
        m_Script.mt_Add_Command(new Command_ShowDialog({"Ça fait plusieurs jours que je suis dans cette grotte...",
                                                       "Mais il me semble que depuis quelques mètres l'odeur a changé.",
                                                       "La sortie ?",
                                                       "Oui !",
                                                       "Enfin !"}));
        m_Script.mt_Add_Command(new Command_Camera(m_Map->m_Tileset->mt_Cell_To_Pixel(m_Player->m_Pos + sf::Vector2f(20, 15)), 2.0f, new Interpolator_Cos, false));
        //m_Script.mt_Add_Command(new Command_Camera(m_Map->m_Tileset->mt_Cell_To_Pixel(m_Player->m_Pos), 1.0f, new Interpolator_Cos));
        m_Script.mt_Add_Command(new Command_Music("Map", 1.0f));
        m_Script.mt_Add_Command(new Command_Lights(sf::Color::Black, sf::Color::White, 2.0f));
        m_Script.mt_Add_Command(new Command_ShowDialog())*/
    }


}

void GameEngine::mt_Play_Music(const std::string& music_id)
{
    if (m_Music != nullptr)
    {
        Context::smt_Get().m_Musics.mt_Release(m_Music);
    }

    m_Music = nullptr;

    if (music_id != "")
    {
        m_Music = Context::smt_Get().m_Musics.mt_Get_Resource(music_id);
        m_Music->setLoop(true);
        m_Music->play();
    }
}

std::string GameEngine::mt_Get_Music_Id(void)
{
    std::string l_Ret;

    if (m_Music != nullptr)
    {
        l_Ret = *m_Music.m_Resource_Id;
    }

    return l_Ret;
}

sf::Music* GameEngine::mt_Get_Music(void)
{
    return m_Music.m_Resource;
}


void GameEngine::mt_End_Game(void)
{
    m_State = GameStateType::EndGame;
}

void GameEngine::mt_End_Demo(void)
{
    m_Script.mt_Add_Command(new Command_Music("", 2.0f));
    //m_Script.mt_Add_Command(new Command_Wait(2.0f));
    m_Script.mt_Add_Command(new Command_ShowDialog({"Cette démonstration touche à sa fin.",
                                                    "Merci d'avoir joué !"}));
    m_Script.mt_Add_Command(new Command_End_Game());
}
