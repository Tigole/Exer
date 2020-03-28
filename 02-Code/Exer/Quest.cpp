#include "Quest.hpp"
#include "Context.hpp"
#include "Command.hpp"
#include "GameEngine.hpp"
#include <sstream>
#include <iomanip>

Quest_Test::Quest_Test()
{
    m_Quest_Id = "First Quest";
}

bool Quest_Test::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature)
{
    bool l_Ret(false);
    return l_Ret;
}

bool Quest_Test::mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id)
{
    if (map_id == "Road")
    {

        //l_dyn->m_Friendly = true;
        //l_dyn->m_Pos = {25, 19};

        //dyns.push_back(std::unique_ptr<Dynamic>(l_dyn));
        m_Phase = 0;
    }
    return true;
}

void Quest_Test::mt_Get_Description(std::vector<std::vector<sf::String>>& description)
{
    description.push_back({"Classe de test", "Pas grand chose à dire"});
    description.push_back({"1ère ligne du 2nd paragraphe", "Pas grand chose à dire"});
}



Quest_Main::Quest_Main()
{
    m_Quest_Id = "L'Aventure commance !";
}

bool Quest_Main::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature)
{
    bool l_b_Ret(false);
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Player = Context::smt_Get().m_Engine->m_Player;
    GameEngine* l_Engine = Context::smt_Get().m_Engine;
    auto l_Dyn = std::find_if(l_Engine->m_Dyn.begin(), l_Engine->m_Dyn.end(), [](const Resource<Dynamic> d){return d->m_Name == "FinLight";});
    Dynamic_Light* l_Light = nullptr;

    if (l_Dyn != l_Engine->m_Dyn.end())
    {
        l_Light = dynamic_cast<Dynamic_Light*>(l_Dyn->m_Resource);
    }

    if (tgt->m_Name == "FinDeJeu")
    {
        if (    1/*(Context::smt_Get().m_Engine->m_System_Quest.mt_Has_Quest("Sombres manigances"))
            &&  (Context::smt_Get().m_Engine->m_System_Quest.mt_Is_Quest_Ended("Sombres manigances"))*/)
        {
            l_Player->m_Desired_Vel = {0.0f, 0.0f};

            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "En route pour " + fn_City_Name("Axâme", sf::Color::White) + " !")}));
            //l_Script->mt_Add_Command(new Command_Lights(l_Engine->m_Sky_Color, sf::Color::Black, 1.0f));
            l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {{3.0f, 0.0f}}, 1.0f, true));
            l_Script->mt_Add_Command(new Command_End_Game());
            m_Completed = true;
        }
        else
        {
            //
        }

        l_b_Ret = true;
    }

    return l_b_Ret;
}

bool Quest_Main::mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id)
{
    return true;
}

void Quest_Main::mt_Get_Description(std::vector<std::vector<sf::String>>& description)
{
    description.push_back({"Un troll me barre le passage pour traverser le pont..."});
}









Quest_Troll::Quest_Troll()
 :  m_Paid(false),
    m_Moving_Player(false)
{
    m_Quest_Id = "Un point bloquant";
}

bool Quest_Troll::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature)
{
    bool l_b_Ret(false);
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Troll = Context::smt_Get().m_Engine->mt_Get_Creature("Troll");
    Creature* l_Player = Context::smt_Get().m_Engine->m_Player;

    if ((l_Troll != nullptr) && (m_Moving_Player == false) && ((tgt->m_Name == "Troll") || (tgt->m_Name == "Troll2")))
    {
        if (l_Troll->m_Gameplay_Data.m_Health > 0)
        {
            std::vector<DialogChoice> l_Choices;

            Context::smt_Get().m_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};

            l_Script->mt_Add_Command(new Command_Music("Dialog_Troll", 0.5f));

            if (tgt->m_Name == "Troll2" && (m_Moving_Player == false))
            {
                l_Troll->mt_LookAt(*l_Player);
                l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "Hé!")}));
                l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(-1, 0), sf::Vector2f(0, 1)}, l_Player->m_Speed * 0.5f, false));
                l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Troll));
                l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, l_Player));
                m_Moving_Player = true;
            }
            l_Troll->mt_LookAt(*l_Player);

            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "Halte !"),
                                                            fn_Dialog(l_Troll, "Tu dois payer une taxe pour passer mon pont !")}));

            l_Choices.push_back(DialogChoice("#white Je ne veux pas d'ennuis.", &Quest_Troll::mt_On_Leave, this));
            l_Choices.push_back(DialogChoice("#red Hors de question!", &Quest_Troll::mt_On_Fight, this));
            if (Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item<Item>("Gold", ItemType::Quest) != nullptr)
            {
                l_Choices.push_back(DialogChoice("#yellow J'ai de l'or si tu veux.", &Quest_Troll::mt_On_Pay, this));
            }
            l_Script->mt_Add_Command(new Command_Choice(l_Choices));
            m_Map_Music_Id = Context::smt_Get().m_Engine->mt_Get_Music_Id();
            l_b_Ret = true;
        }
    }

    return l_b_Ret;
}

bool Quest_Troll::mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id)
{
    return true;
}

void Quest_Troll::mt_Get_Description(std::vector<std::vector<sf::String>>& description)
{
    description.push_back({"Un troll me barre la route."});
    description.push_back({"Je pense pouvoir le payer ou utiliser la manière forte pour passer ce pont."});
}

void Quest_Troll::mt_On_Leave(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(Context::smt_Get().m_Engine->mt_Get_Creature("Troll"), "J'aime mieux ça!")}));
    l_Script->mt_Add_Command(new Command_Music(m_Map_Music_Id, 1.0f));
    m_Moving_Player = false;
}

void Quest_Troll::mt_On_Fight(void)
{
    std::map<int, std::vector<CommandFightCreature>> l_Fighters;
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    GameEngine* l_Engine = Context::smt_Get().m_Engine;
    Creature* l_Creature;
    CommandFightCreature l_Tmp;

    l_Creature = l_Engine->mt_Get_Creature("player");
    l_Tmp.m_Tgt = l_Creature;
    l_Tmp.m_Logic_Id = SystemFight::m_Human_Logic_String;
    l_Tmp.m_Actions_Count = 2;
    l_Fighters[0].push_back(l_Tmp);

    l_Creature = l_Engine->mt_Get_Creature("Troll");
    l_Tmp.m_Tgt = l_Creature;
    l_Tmp.m_Logic_Id = "Dumb";
    l_Tmp.m_Actions_Count = 1;
    l_Fighters[1].push_back(l_Tmp);

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Engine->mt_Get_Creature("Troll"), "Hahaha!"),
                                                    fn_Dialog(l_Engine->mt_Get_Creature("Troll"), "Et tu vas faire quoi!?")}));
    l_Script->mt_Add_Command(new Command_Fight(l_Fighters, std::bind(&Quest_Troll::mt_On_Victory, this), "Fight_Troll"));
    m_Moving_Player = false;
}

void Quest_Troll::mt_On_Victory(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Player = Context::smt_Get().m_Engine->mt_Get_Creature("player");

    l_Script->mt_Add_Command(new Command_Music("", 1.0f));
    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Repose en paix gardien du pont.")}));
    l_Script->mt_Add_Command(new Command_Music(m_Map_Music_Id, 1.0f));
}

void Quest_Troll::mt_On_Pay(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Troll = Context::smt_Get().m_Engine->mt_Get_Creature("Troll");

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "De l'or!"),
                                                    fn_Dialog(l_Troll, "Tu peux passer.")}));
    l_Script->mt_Add_Command(new Command_MoveOffset(l_Troll, {sf::Vector2f(0, 1)}, l_Troll->m_Speed, false));
    l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, sf::Vector2f(0.0f, -1.0f), true));
    l_Script->mt_Add_Command(new Command_Music(m_Map_Music_Id, 1.0f));
    Context::smt_Get().m_Engine->m_Inventory.mt_Change_Item_Count("Gold", -1, ItemType::Quest);

    m_Completed = true;
    m_Moving_Player = false;
}



Quest_Dark_Mage::Quest_Dark_Mage()
 :  m_Phase(0), m_Started(false), m_Troll_Asked(false), m_Troll_Help(false), m_Villager_Help(false), m_Mage_Seen(false)
{
    m_Quest_Id = "Sombres manigances";
}

bool Quest_Dark_Mage::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature)
{
    bool l_b_Ret(false);
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    GameEngine* l_Engine = Context::smt_Get().m_Engine;
    Creature* l_Player = l_Engine->m_Player;
    Creature* l_Villageois2 = l_Engine->mt_Get_Creature("Villageois2");
    Creature* l_Villageois_Blesse = l_Engine->mt_Get_Creature("Villageois1");
    Creature* l_Bob = l_Engine->mt_Get_Creature("Marbobley");
    Creature* l_Troll = l_Engine->mt_Get_Creature("Troll");
    sf::String l_Color_Str = fn_Color_To_RichText(50, 205, 50);

    if ((m_Started == false) && (tgt->m_Name == "SombreManigances_Villageois"))
    {
        l_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_ShowDialog({"???\nA l'aide !"}));
        l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(102.0f, 55.0f), l_Player->m_Speed * 1.7f, false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Villageois2));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Mais que se passe-t-il ?"),
                                                        fn_Dialog(l_Villageois2, "On était au sanctuaire en train de faire nos prières et"),
                                                        fn_Dialog(l_Villageois2, "Oh mon dieu !"),
                                                        fn_Dialog(l_Villageois2, "On s'est fait attaqué !"),
                                                        fn_Dialog(l_Villageois2, l_Villageois_Blesse->m_Name + " est blessé !"),
                                                        fn_Dialog(l_Villageois2, "Il faut l'emmener au village sans attendre !")}));
        l_Script->mt_Add_Command(new Command_Lights(l_Engine->m_Sky_Color, sf::Color::Black, 1.0f));
        l_Script->mt_Add_Command(new Command_SetPos(l_Villageois2, sf::Vector2f(97.0f, 100.0f)));
        l_Script->mt_Add_Command(new Command_SetPos(l_Villageois_Blesse, sf::Vector2f(97.0f, 99.0f)));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, sf::Vector2f(1.0f, 0.0f), true));
        l_Script->mt_Add_Command(new Command_Lights(sf::Color::Black, l_Engine->m_Sky_Color, 1.0f));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je suppose que ce sont des habitants de " + fn_City_Name("La Passe")+ "."),
                                                        fn_Dialog(l_Player, "Ce vilage se situe au Sud de ma position.")}));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(0.0f, 1.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Si ces gens sont en danger,\nil est de mon devoir de les aider."),
                                                        fn_Dialog(l_Player, "Il serait intéressant d'en savoir plus sur leur agresseur.")}));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(0.0f, -1.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "D'un autre côté, il semble se passer des\nchoses anormales à leur sanctuaire."),
                                                        fn_Dialog(l_Player, "Si je n'interviens pas rapidement,\nil peut y avoir d'autres victimes."),
                                                        fn_Dialog(l_Player, "C'est un choix bien cornélien.")}));
        l_b_Ret = true;
        m_Started = true;
    }
    else if ((m_Started == true) && (m_Villager_Help == false) && (m_Villager_Seen == false) && ((tgt->m_Name == l_Villageois2->m_Name) || (tgt->m_Name == l_Bob->m_Name)))
    {
        l_Player->m_Desired_Vel = {0.0f, 0.0f};
        m_Villager_Seen = true;
        if (l_Player->m_Pos.x < l_Villageois2->m_Pos.x)
        {
            l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(l_Villageois2->m_Pos.x, l_Villageois2->m_Pos.y + 2.0f), l_Player->m_Speed, false));
        }
        l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(98.0f, 100.0f), l_Player->m_Speed, false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(0.0f, -1.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Aide-nous guérisseur !"),
                                                        fn_Dialog(l_Villageois2, l_Villageois_Blesse->m_Name + " souffre terriblement !"),
                                                        fn_Dialog(l_Villageois_Blesse, "Oh mon dieux !!!\nQue je souffre le martyr !"),
                                                        fn_Dialog(l_Villageois2, "Tu vois ?"),
                                                        fn_Dialog(l_Bob, "Cement-dou mec..."),
                                                        fn_Dialog(l_Bob, "Il a pas l'air bien mais cement-dou..."),
                                                        fn_Dialog(l_Bob, "Malheureusement je n'ai plus " + fn_Quest_Object("d'herbe magique") + "\npour l'aider à ne plus sentir la douleur...")
                                                        }));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Bob, l_Player));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Bob, "T'es nouveau toi ?"),
                                                        fn_Dialog(l_Bob, "Tu veux une taffe de bienvenue ?"),
                                                        fn_Dialog(l_Player, "Oui."),
                                                        fn_Dialog(l_Player, "Quoi ???"),
                                                        fn_Dialog(l_Bob, "Pour te détendre mec."),
                                                        fn_Dialog(l_Bob, "Tu m'as l'air bien tendu..."),
                                                        fn_Dialog(l_Player, "Merci mais je ne n'use jamais de cette magie païenne."),
                                                        fn_Dialog(l_Villageois2, "Mais je croyais que t'en avais plus ?"),
                                                        fn_Dialog(l_Bob, "Ah ouais c'est vrai..."),
                                                        fn_Dialog(l_Villageois2, "...")}));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Player));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Villageois2));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Désolé pour lui."),
                                                        fn_Dialog(l_Player, "Ne vous en faites pas.\nJ'ai traversé moultes épreuves plus torturantes."),
                                                        fn_Dialog(l_Player, "Je suis intéressé par tout ce que vous\npourrez me dire sur votre agression."),
                                                        fn_Dialog(l_Villageois2, "On était au sanctuaire avec " + l_Villageois_Blesse->m_Name + " et on s'est fait attaqué."),
                                                        fn_Dialog(l_Villageois2, "On a déguerpit aussi vite que possible et on est tombé sur vous."),
                                                        fn_Dialog(l_Villageois2, "La suite, vous la connaissez."),
                                                        fn_Dialog(l_Player, "Intéressant."),
                                                        fn_Dialog(l_Player, "Quelqu'un peut avoir des raisons de vous en vouloir ?"),
                                                        fn_Dialog(l_Villageois2, "Non."),
                                                        fn_Dialog(l_Villageois2, "Et puis tout le monde est en ville en ce moment."),
                                                        fn_Dialog(l_Villageois2, "Qu'allez-vous faire ?"),
                                                        fn_Dialog(l_Player, "Tout semble indiqué que la suite est\nd'aller voir ce qu'il se trame au sanctaire."),
                                                        fn_Dialog(l_Villageois2, "Vous avez raison."),
                                                        fn_Dialog(l_Villageois2, "Néanmoins, si vous trouvez de " + fn_Quest_Object("l'herbe magique") + ",\nje serais plus serein et je pourrais vous accompagner."),
                                                        fn_Dialog(l_Player, "Merci à vous."),
                                                        fn_Dialog(l_Villageois2, "Non."),
                                                        fn_Dialog(l_Villageois2, "Merci à vous.")}));

        l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(0, 2)}, l_Player->m_Speed / 2.0f, false));
        if ((l_Troll != nullptr) && (l_Troll->m_Gameplay_Data.m_Health > 0))
        {
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Il semble que les habitants de cette forêt\nsoient plus fier qu'il n'y parait."),
                                                            fn_Dialog(l_Player, "Peut-être que ce troll croisé précédemment\npourrais nous aider.")}));
        }
        l_b_Ret = true;
    }
    else if ((m_Started == true) && (m_Villager_Seen == true) && (tgt->m_Name == l_Villageois2->m_Name))
    {
        l_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Player));
        if (l_Engine->m_Inventory.mt_Get_Item_Count("Medicinal_Herb", ItemType::Quest) > 0)
        {
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je viens avec de " + fn_Quest_Object("l'herbe magique") + "."),
                                                            fn_Dialog(l_Villageois2, "Mon seigneur, vous êtes trop bon !")}));
            l_Engine->m_Inventory.mt_Change_Item_Count("Medicinal_Herb", -1, ItemType::Quest);
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Je vous attend au sanctaire."),
                                                            fn_Dialog(l_Villageois2, "Ne tardez pas trop.")}));
            l_Script->mt_Add_Command(new Command_Lights(sf::Color::White, sf::Color::Black, 0.2f));
            l_Script->mt_Add_Command(new Command_SetPos(l_Villageois2, sf::Vector2f(69.0f, 19.0f)));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, sf::Vector2f(-1.0f, 0.0f), true));
            l_Script->mt_Add_Command(new Command_Lights(sf::Color::Black, sf::Color::White, 0.2f));
            m_Villager_Help = true;
            l_b_Ret = true;
        }
        else if (m_Villager_Help == false)
        {
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "N'oubliez pas d'apporter de " + fn_Quest_Object("l'herbe magique") + " !")}));
            l_b_Ret = true;
        }
    }
    else if ((m_Started == true) && (m_Troll_Asked == false) && (tgt->m_Name == l_Troll->m_Name))
    {
        l_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "Que veux-tu ?\nTu m'a déjà donné de l'or."),
                                                        fn_Dialog(l_Player, "Un mal ronge cette forêt."),
                                                        fn_Dialog(l_Player, "Si personne ne s'en occupe,\nles être peuplant cet endroit risquent\nde connaître un sort sinistre."),
                                                        fn_Dialog(l_Troll, "C'est bon, je suis avec toi."),
                                                        fn_Dialog(l_Player, "C'est pourquoi je viens quérir ton aide\ngardien du pont de la rivière !"),
                                                        fn_Dialog(l_Troll, "Je t'ai dit que tu pouvais compter sur moi."),
                                                        fn_Dialog(l_Player, "Seule une âme pure ..."),
                                                        fn_Dialog(l_Troll, "STOOOOOOOOPPPP"),
                                                        fn_Dialog(l_Troll, "JE VAIS T'AIDER !!!!"),
                                                        fn_Dialog(l_Player, "..."),
                                                        fn_Dialog(l_Player, "C'est fort aimable à toi !"),
                                                        fn_Dialog(l_Troll, "...")}));
        l_Script->mt_Add_Command(new Command_Lights(sf::Color::White, sf::Color::Black, 0.2f));
        l_Script->mt_Add_Command(new Command_SetPos(l_Troll, sf::Vector2f(69.0f, 21.0f)));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, sf::Vector2f(-1.0f, 0.0f), true));
        l_Script->mt_Add_Command(new Command_Lights(sf::Color::Black, sf::Color::White, 0.2f));

        m_Troll_Asked = true;
        m_Troll_Help = true;
        l_b_Ret = true;
    }
    else if ((tgt->m_Name == "SombreManigances_Mage") && (m_Mage_Seen == false))
    {
        std::map<int, std::vector<CommandFightCreature>> l_Fight;
        Creature* l_Mage = l_Engine->mt_Get_Creature("Nivalys");

        l_Player->m_Desired_Vel = {0.0f, 0.0f};

        l_Fight[1].push_back(CommandFightCreature());
        l_Fight[1][0].m_Tgt = l_Player;
        l_Fight[1][0].m_Logic_Id = SystemFight::m_Human_Logic_String;
        l_Fight[1][0].m_Actions_Count = 2;

        l_Fight[2].push_back(CommandFightCreature());
        l_Fight[2][0].m_Tgt = l_Mage;
        l_Fight[2][0].m_Logic_Id = SystemFight::m_Boss_Logic_String;
        l_Fight[2][0].m_Actions_Count = 2;

        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Mage, sf::Vector2f(-1.0f, 0.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Hein ?")}));
        l_Script->mt_Add_Command(new Command_Music("", 2.0f));
        l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(67.0f, 20.0f), l_Player->m_Speed, false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(-1.0f, 0.0f), true));

        if (m_Villager_Help == true)
        {
            l_Fight[1].push_back(CommandFightCreature());
            l_Fight[1].back().m_Tgt = l_Villageois2;
            l_Fight[1].back().m_Logic_Id = SystemFight::m_Villager_Logic_String;
            l_Fight[1].back().m_Actions_Count = 2;

            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Villageois2));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Player));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Mais qui est cet homme ?"),
                                                            fn_Dialog(l_Player, "On va vite le découvrir !")}));
        }

        if (m_Troll_Help == true)
        {
            l_Fight[1].push_back(CommandFightCreature());
            l_Fight[1].back().m_Tgt = l_Villageois2;
            l_Fight[1].back().m_Logic_Id = SystemFight::m_Villager_Logic_String;
            l_Fight[1].back().m_Actions_Count = 2;

            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Troll));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, l_Player));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "Il est comme ça depuis tout à l'heure..."),
                                                            fn_Dialog(l_Troll, "Mais que fait-il ?"),
                                                            fn_Dialog(l_Player, "On dirait qu'il incante un sort ?!")}));
        }

        if ((m_Troll_Help == true) && (m_Villager_Help == true))
        {
            //
        }

        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Peut-être que cet homme est au courant\nde ce qui est arrivé aux villageois."),
                                                        fn_Dialog(l_Player, "J'ai quelques questions à lui poser.")}));
        l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(55.0f, 20.0f), l_Player->m_Speed, false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Mage));
        if (m_Troll_Help == true)
        {
            l_Script->mt_Add_Command(new Command_MoveTo(l_Troll, sf::Vector2f(54.0f, 20.0f), l_Troll->m_Speed, false));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, l_Mage));
        }
        if (m_Villager_Help == true)
        {
            l_Script->mt_Add_Command(new Command_MoveTo(l_Villageois2, sf::Vector2f(55.0f, 20.0f), l_Villageois2->m_Speed, false));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Mage));
        }

        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Bonsoir.")}));
        l_Script->mt_Add_Command(new Command_Wait(1.5f));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Oui ?")}));
        if (0)
        {
            //
        }
        else
        {
            l_Script->mt_Add_Command(new Command_Wait(1.0f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Des habitants de " + fn_City_Name("La Passe")+ "\nont étés attaqué plus tôt dans la journée."),
                                                            fn_Dialog(l_Player, "Avez-vous vu quelque-chose ?")}));
            l_Script->mt_Add_Command(new Command_Wait(1.0f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Maintenant que vous le dîtes.")}));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Mage, l_Player));
            l_Script->mt_Add_Command(new Command_Music("Map_Boss", 0.5f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Il me semble avoir entendu du bruit."),
                                                            fn_Dialog(l_Player, "Il y a longtemps ?"),
                                                            fn_Dialog(l_Mage, "Longtemps ?")}));
            l_Script->mt_Add_Command(new Command_Wait(1.0f));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Mage, sf::Vector2f(-1.0f, 0.0f), true));
            l_Script->mt_Add_Command(new Command_Wait(0.7f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Je ne sais pas."),
                                                            fn_Dialog(l_Player, "Comment ne pouvez-vous pas savoir ?"),
                                                            fn_Dialog(l_Player, "Vous étiez là ou non ?")}));
            l_Script->mt_Add_Command(new Command_Wait(1.0f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Ecoutez, je n'ai rien à vous dire !"),
                                                            fn_Dialog(l_Player, "Vous m'avez l'air d'en savoir plus qu'il n'y parait."),
                                                            fn_Dialog(l_Player, "Que faîtes-vous ici ?\nTout seul ?\nLa nuit ?")}));
            l_Script->mt_Add_Command(new Command_Wait(0.2f));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Mage, l_Player));
            l_Script->mt_Add_Command(new Command_Wait(0.2f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Je pêche des écrevisses."),
                                                            fn_Dialog(l_Player, "C'est ça."),
                                                            fn_Dialog(l_Player, "Et moi je suis votre tante."),
                                                            fn_Dialog(l_Player, "Aller, dîtes-moi la vérité."),
                                                            fn_Dialog(l_Player, "C'est vous qui avez attaqué ces pauvres villageois.")}));
        }

        l_Script->mt_Add_Command(new Command_Wait(1.5f));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Vous n'auriez jamais dû venir ici !"),
                                                        fn_Dialog(l_Player, "Permettez-moi d'en douter.")}));
        l_Script->mt_Add_Command(new Command_Music("", 0.5f));
        l_Script->mt_Add_Command(new Command_Fight(l_Fight, std::bind(Quest_Dark_Mage::mt_On_Victory, this), "Fight_Boss"));

        m_Mage_Seen  = true;
        l_b_Ret = true;
    }

    return l_b_Ret;
}

bool Quest_Dark_Mage::mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id)
{
    bool l_b_Ret(false);

    return l_b_Ret;
}

void Quest_Dark_Mage::mt_On_Victory(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Player = Context::smt_Get().m_Engine->mt_Get_Creature("player");

    l_Script->mt_Add_Command(new Command_Music("", 1.0f));
    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Les habitants de " + fn_City_Name("La Passe")+ " ne seront plus dérangés\npar cet homme."),
                                                    fn_Dialog(l_Player, "J'en ai fini avec cet endroit."),
                                                    fn_Dialog(l_Player, "Je peux maintenant avancer en direction d'" + fn_City_Name("Axâme")+ ", vers l'Est.")}));
    l_Script->mt_Add_Command(new Command_Music(Context::smt_Get().m_Engine->m_Map->m_Name, 1.0f));

    m_Completed = true;
}

void Quest_Dark_Mage::mt_Get_Description(std::vector<std::vector<sf::String>>& description)
{
    //
}






Quest_Tutoriel::Quest_Tutoriel()
{
    m_Quest_Id = "Tutoriel";

    m_Wolf_Seen = false;
    m_Wolf_Defeated = false;
    m_Chest_Seen = false;
    m_Chest_Open = false;
}

bool Quest_Tutoriel::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature)
{
    bool l_Ret(false);
    bool l_Wolf_Warn = tgt->m_Name.find("Tutoriel_Wolf_Warn") != std::string::npos;
    GameEngine* l_Engine = Context::smt_Get().m_Engine;
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Player = l_Engine->m_Player;
    sf::Color l_Player_Color(sf::Color::Cyan);

    if ((tgt->m_Name == "Tutoriel_Wolf_Warn") && (m_Wolf_Seen == false))
    {
        l_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Attention au loup devant moi."),
                                  fn_Dialog(l_Player, "Ca va être mon premier combat."),
                                  fn_Dialog(l_Player, "Je dois bien me décider à attaquer\nlorqu'il est à porter."),
                                  fn_Dialog(l_Player, "Je peux également me déplacer pour\nm'approcher de lui\nou au contraire m'en éloigner."),
                                  fn_Dialog(l_Player, "Dans tous les cas, j'ai l'avantage\nde décider en premier\nde ce que je vais faire."),}));
        m_Wolf_Seen = true;
    }
    else if (((tgt->m_Name == "Wolf") || (tgt->m_Name == "Tutoriel_Wolf_Fight")) && (m_Wolf_Defeated == false))
    {
        std::map<int, std::vector<CommandFightCreature>> l_Parties;
        Creature* l_Wolf = Context::smt_Get().m_Engine->mt_Get_Creature("Wolf");
        l_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Wolf->mt_LookAt(*l_Engine->m_Player);

        l_Parties[0].resize(1);
        l_Parties[0][0].m_Tgt = l_Engine->m_Player;
        l_Parties[0][0].m_Logic_Id = "Human";
        l_Parties[0][0].m_Actions_Count = 2;

        l_Parties[1].resize(1);
        l_Parties[1][0].m_Tgt = l_Wolf;
        l_Parties[1][0].m_Logic_Id = "Dumb";
        l_Parties[1][0].m_Actions_Count = 2;

        l_Script->mt_Add_Command(new Command_Fight(l_Parties, std::bind(Quest_Tutoriel::mt_On_Victory, this), "Fight_Wolf"));
        m_Wolf_Defeated = true;
        l_Ret = true;
    }
    else if ((tgt->m_Name == "Tutoriel_Chest_Warn") && (m_Chest_Seen == false))
    {
        l_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Vers l'Est se trouve " + fn_City_Name("Axâme")+ ",\nma destination.")}));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Engine->m_Player, sf::Vector2f(0.0f, 1.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je devrais tout de même regarder où mène ce chemin au Sud."),
                                                         fn_Dialog(l_Player, "Histoire d'être sur de ne rien rater.")}));
        m_Chest_Seen = true;
    }
    else if (tgt->m_Name == "Chest")
    {
        l_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};
        /// Open chest
        l_Ret = true;
    }

    if ((m_Wolf_Defeated == true) && (m_Chest_Open == true))
    {
        m_Completed = true;
    }

    return l_Ret;
}

void Quest_Tutoriel::mt_On_Victory(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Player = Context::smt_Get().m_Engine->mt_Get_Creature("player");
    SystemInventory& l_Invetory = Context::smt_Get().m_Engine->m_Inventory;
    int l_Life_Count = 20;
    int l_Psy_Count = 20;

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Ah !"),
                                                    fn_Dialog(l_Player, "La sale bête !")}));


    /*Context::smt_Get().m_Engine->mt_End_Demo();
    return;*/
    if (    (l_Player->m_Gameplay_Data.m_Health != l_Player->m_Gameplay_Data.m_Health_Max)
        ||  (l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy != l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max))
    {
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je ferais bien de me soigner\navant de tomber sur une autre bestiole.")}));

        l_Invetory.mt_Change_Item_Count("LifePotion", l_Life_Count, ItemType::Edible);
        l_Invetory.mt_Change_Item_Count("PsyPotion", l_Psy_Count, ItemType::Edible);

        l_Script->mt_Add_Command(new Command_ShowDialog({"~La touche " + fn_Command_Key("I") + " permet d'accéder à l'inventaire.~",
                                                        "~Une fois dans l'inventaire, la navigation se fait avec\nles " + fn_Command_Key("touches directionnelles") + ".~",
                                                        "~Pour sélectionner un menu ou un item, appuyez sur " + fn_Command_Key("Espace") + ".~"}));

        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "La prochaine fois, je devrais utiliser\nma compétence spéciale pour en finir plus vite.")}));
        l_Script->mt_Add_Command(new Command_AddSkill(l_Player, Context::smt_Get().m_Engine->m_Player_Skill_Id));
    }
}

bool Quest_Tutoriel::mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id)
{
    /*if (m_Wolf_Defeated == true)
    {
        auto l_wolf = std::find_if(dyns.begin(), dyns.end(), [&](const Resource<Dynamic>& d){return d->m_Name == "Wolf";});
        if (l_wolf != dyns.end())
        {
            dyns.erase(l_wolf);
        }
    }
    if (m_Chest_Open)
    {
        //
    }*/
    return true;
}

void Quest_Tutoriel::mt_Get_Description(std::vector<std::vector<sf::String>>& description)
{
    //
}

