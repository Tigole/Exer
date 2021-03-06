#include "Quest.hpp"
#include "Context.hpp"
#include "Command.hpp"
#include "GameEngine.hpp"
#include <sstream>
#include <iomanip>

Quest_Main::Quest_Main()
{
    m_Quest_Name = "L'Aventure commence !";
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
        m_Completed = Context::smt_Get().m_Engine->m_System_Quest.mt_Is_Quest_Ended("Sombres_Manigances")
                        && Context::smt_Get().m_Engine->m_System_Quest.mt_Is_Quest_Ended("Troll_Quest");
        if (m_Completed)
        {
            l_Player->m_Desired_Vel = {0.0f, 0.0f};

            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "En route pour " + fn_City_Name("Ax�me", sf::Color::White) + " !")}));
            l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {{2.5f, 0.0f}}, 0.5f, true));
            l_Script->mt_Add_Command(new Command_End_Game());
            m_Completed = true;
        }
        else
        {
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je ne peux pas laisser les villageois\nsans rien faire."),
                                                            fn_Dialog(l_Player, "Je dois les aider !")}));
            l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(141.0f, 50.0f), l_Player->m_Speed, false));
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
    SystemQuest* l_Quests = &Context::smt_Get().m_Engine->m_System_Quest;
    const Quest_Dark_Mage* l_Dark_Mage = nullptr;
    const Quest_Tutoriel* l_Tutoriel = nullptr;
    const Quest_Troll* l_Troll = nullptr;

    l_Dark_Mage = l_Quests->mt_Get_Quest<Quest_Dark_Mage>("Sombres_Manigances");
    l_Troll = l_Quests->mt_Get_Quest<Quest_Troll>("Troll_Quest");
    l_Tutoriel = l_Quests->mt_Get_Quest<Quest_Tutoriel>("Tutoriel");

    if (l_Dark_Mage != nullptr)
    {
        if (l_Dark_Mage->m_Completed)
        {
            description.push_back({"J'ai r�solu le myst�re de l'attaque des villageois."});
        }
        else
        {
            description.push_back({"Les villageois de " + fn_City_Name("La Passe") + " ont �t� attaqu�s.",
                                  "Je ne peux pas rester sans rien faire."});
        }
    }

    if (l_Troll != nullptr)
    {
        if (l_Troll->m_Completed == true)
        {
            std::vector<sf::String> l_Text;

            l_Text.push_back("J'ai r�ussi � traverser le pont.");

            if (l_Troll->m_Paid == true)
            {
                l_Text.push_back("J'ai perdu ma pi�ce d'or mais j'en trouverai certainement d'autres.");
            }
            else
            {
                l_Text;
            }
            description.push_back(l_Text);
        }
        else
        {
            description.push_back({"Un troll m'emp�che de traverser le pont..."});
        }
    }

    if (l_Tutoriel != nullptr)
    {
        if (l_Tutoriel->m_Completed == true)
        {
            description.push_back({"J'ai termin� le tutoriel !"});
        }
        else
        {
            description.push_back({"Un peu d'exercice me fera du bien.",
                                  "J'ai besoin de me familiariser avec l'environnement"});
        }
    }
}









Quest_Troll::Quest_Troll()
 :  m_Paid(false),
    m_Moving_Player(false)
{
    m_Quest_Name = "Un point bloquant";
}

bool Quest_Troll::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature)
{
    bool l_b_Ret(false);
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Troll = Context::smt_Get().m_Engine->mt_Get_Creature("Troll");
    Creature* l_Player = Context::smt_Get().m_Engine->m_Player;

    if ((m_Completed == false) && (l_Troll != nullptr) && (m_Moving_Player == false) && ((tgt->m_Name == "Troll") || (tgt->m_Name == "Troll2")))
    {
        if (l_Troll->m_Gameplay_Data.m_Health > 0)
        {
            std::vector<DialogChoice> l_Choices;
            const Item* l_Item;

            Context::smt_Get().m_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};

            l_Script->mt_Add_Command(new Command_Music("Dialog_Troll", 0.5f));

            if (tgt->m_Name == "Troll2" && (m_Moving_Player == false))
            {
                l_Troll->mt_LookAt(*l_Player);
                l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "H�!")}));
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
            l_Item = Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item<Item>("Gold", ItemType::Quest);
            if ((l_Item != nullptr) && (l_Item->m_Quantity > 0))
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
    if (m_Completed == true)
    {
        description.push_back({"J'ai r�ussi � passer le pont."});
    }

    description.push_back({"Un troll me barre la route."});
    description.push_back({"Je peux me battre contre lui.","Il me semble �galement que les trolls app�cient l'or."});
}

void Quest_Troll::mt_On_Leave(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(Context::smt_Get().m_Engine->mt_Get_Creature("Troll"), "J'aime mieux �a!")}));
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
    l_Tmp.m_Logic_Id = SystemFight::ms_Human_Logic_String;
    l_Tmp.m_Actions_Count = 2;
    l_Fighters[1].push_back(l_Tmp);

    l_Creature = l_Engine->mt_Get_Creature("Troll");
    l_Tmp.m_Tgt = l_Creature;
    l_Tmp.m_Logic_Id = SystemFight::ms_Dumb_Logic_String;
    l_Tmp.m_Actions_Count = 1;
    l_Fighters[2].push_back(l_Tmp);

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Engine->mt_Get_Creature("Troll"), "Hahaha!"),
                                                    fn_Dialog(l_Engine->mt_Get_Creature("Troll"), "Et tu vas faire quoi!?")}));
    l_Script->mt_Add_Command(new Command_Fight(l_Fighters, std::bind(&Quest_Troll::mt_On_Victory, this), "Fight_Troll"));
    m_Moving_Player = false;
}

void Quest_Troll::mt_On_Victory(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Player = Context::smt_Get().m_Engine->mt_Get_Creature("player");
    Creature* l_Germain = Context::smt_Get().m_Engine->mt_Get_Creature("Germain");
    Creature* l_Germaine = Context::smt_Get().m_Engine->mt_Get_Creature("Germaine");

    l_Script->mt_Add_Command(new Command_Music("", 1.0f));
    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Repose en paix gardien du pont.")}));
    l_Script->mt_Add_Command(new Command_Music(m_Map_Music_Id, 1.0f));
    l_Script->mt_Add_Command(new Command_MoveOffset(l_Germain, {sf::Vector2f(0.0f, 1.0f)}, l_Germain->m_Speed, false));
    l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Germain, l_Player));
    l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Germain));
    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Germain, "Merci mon seigneur."),
                                                    fn_Dialog(l_Germain, "Tenez. Prennez ceci en gage de remerciement.")}));
    fn_Command_Give_Items(l_Script, {ChestData("LifePotion", ItemType::Edible, 2)});
    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Merci � vous."),
                                                    fn_Dialog(l_Player, "Mais vous savez, je n'ai fait que mon devoir."),
                                                    fn_Dialog(l_Germain, "On peut enfin aller � " + fn_City_Name("Ax�me") + ".")}));
    l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Germain, l_Germaine));
    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Germain, "Aller, on y va.")}));
    l_Script->mt_Add_Command(new Command_Lights(sf::Color::White, sf::Color::Black, 0.5f));
    l_Script->mt_Add_Command(new Command_SetPos(l_Germain, sf::Vector2f(149.0f, 0.0f)));
    l_Script->mt_Add_Command(new Command_SetPos(l_Germaine, sf::Vector2f(149.0f, 0.0f)));
    l_Script->mt_Add_Command(new Command_Lights(sf::Color::Black, sf::Color::White, 0.5f));

    m_Completed = true;
}

void Quest_Troll::mt_On_Pay(void)
{
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    Creature* l_Troll = Context::smt_Get().m_Engine->mt_Get_Creature("Troll");

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "De l'or!"),
                                                    fn_Dialog(l_Troll, "Tu peux passer."),
                                                    fn_Dialog(l_Troll, "Par contre pas les autres.")}));
    l_Script->mt_Add_Command(new Command_MoveTo(l_Troll, sf::Vector2f(33.0f, 63.0f), l_Troll->m_Speed, false));
    l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, sf::Vector2f(0.0f, -1.0f), true));
    l_Script->mt_Add_Command(new Command_Music(m_Map_Music_Id, 1.0f));
    Context::smt_Get().m_Engine->m_Inventory.mt_Change_Item_Count("Gold", -1, ItemType::Quest);

    m_Paid = true;

    m_Completed = true;
    m_Moving_Player = false;
}



Quest_Dark_Mage::Quest_Dark_Mage()
 :  m_Phase(0), m_Started(false), m_Troll_Asked(false), m_Troll_Help(false), m_Villager_Help(false), m_Mage_Seen(false)
{
    m_Quest_Name = "Sombres manigances";
    m_Mage_Defeated_Before_Herbe = false;
}

bool Quest_Dark_Mage::mt_On_Interaction(std::vector<Resource<Dynamic>>& dyns, Dynamic* tgt, Quest_Nature nature)
{
    bool l_b_Ret(false);
    SystemScript* l_Script = &Context::smt_Get().m_Engine->m_Script;
    GameEngine* l_Engine = Context::smt_Get().m_Engine;
    Creature* l_Player = l_Engine->m_Player;
    Creature* l_Villageois2 = l_Engine->mt_Get_Creature("Arthur");
    Creature* l_Villageois_Blesse = l_Engine->mt_Get_Creature("Armand");
    Creature* l_Bob = l_Engine->mt_Get_Creature("Marbobley");
    Creature* l_Troll = l_Engine->mt_Get_Creature("Troll");
    sf::String l_Color_Str = fn_Color_To_RichText(50, 205, 50);

    if ((m_Started == false) && (tgt->m_Name == "SombreManigances_Villageois"))
    {
        l_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_Music("", 0.5f));
        l_Script->mt_Add_Command(new Command_ShowDialog({"???\nA l'aide !"}));
        l_Script->mt_Add_Command(new Command_Music("Introduction", 0.1f));
        l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(102.0f, 55.0f), l_Player->m_Speed * 1.7f, false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Villageois2));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Mais que se passe-t-il ?"),
                                                        fn_Dialog(l_Villageois2, "On �tait au sanctuaire en train de faire nos pri�res et"),
                                                        fn_Dialog(l_Villageois2, "Oh mon dieu !"),
                                                        fn_Dialog(l_Villageois2, "On s'est fait attaqu� !"),
                                                        fn_Dialog(l_Villageois2, l_Villageois_Blesse->m_Name + " est bless� !"),
                                                        fn_Dialog(l_Villageois2, "Il faut l'emmener au village sans attendre !")}));
        l_Script->mt_Add_Command(new Command_Lights(l_Engine->m_Sky_Color, sf::Color::Black, 1.0f));
        l_Script->mt_Add_Command(new Command_SetPos(l_Villageois2, sf::Vector2f(97.0f, 100.0f)));
        l_Script->mt_Add_Command(new Command_SetPos(l_Villageois_Blesse, sf::Vector2f(97.0f, 99.0f)));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, sf::Vector2f(1.0f, 0.0f), true));
        l_Script->mt_Add_Command(new Command_Lights(sf::Color::Black, l_Engine->m_Sky_Color, 1.0f));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je suppose que ce sont des habitants de " + fn_City_Name("La Passe")+ "."),
                                                        fn_Dialog(l_Player, "Ce village se situe au Sud de ma position.")}));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(0.0f, 1.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Si ces gens sont en danger,\nil est de mon devoir de les aider."),
                                                        fn_Dialog(l_Player, "Il serait int�ressant d'en savoir plus sur leur agresseur.")}));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(0.0f, -1.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "D'un autre c�t�, il semble se passer des\nchoses anormales � leur sanctuaire."),
                                                        fn_Dialog(l_Player, "Si je n'interviens pas rapidement,\nil peut y avoir d'autres victimes."),
                                                        fn_Dialog(l_Player, "C'est un choix bien corn�lien.")}));
        l_Script->mt_Add_Command(new Command_Music("Map", 1.0f));
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
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Aide-nous gu�risseur !"),
                                                        fn_Dialog(l_Villageois2, l_Villageois_Blesse->m_Name + " souffre terriblement !"),
                                                        fn_Dialog(l_Villageois_Blesse, "Oh mon dieux !!!\nQue je souffre le martyr !"),
                                                        fn_Dialog(l_Villageois2, "Tu vois ?"),
                                                        fn_Dialog(l_Bob, "Cement-dou mec..."),
                                                        fn_Dialog(l_Bob, "Il a pas l'air bien mais cement-dou..."),
                                                        fn_Dialog(l_Bob, "Malheureusement je n'ai plus " + fn_Quest_Object("d'herbe magique") + "\npour l'aider � ne plus sentir la douleur...")
                                                        }));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Bob, l_Player));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Bob, "T'es nouveau toi ?"),
                                                        fn_Dialog(l_Bob, "Tu veux une taffe de bienvenue ?"),
                                                        fn_Dialog(l_Player, "Oui."),
                                                        fn_Dialog(l_Player, "Quoi ???"),
                                                        fn_Dialog(l_Bob, "Pour te d�tendre mec."),
                                                        fn_Dialog(l_Bob, "Tu m'as l'air bien tendu..."),
                                                        fn_Dialog(l_Player, "Merci mais je ne n'use jamais de cette magie pa�enne."),
                                                        fn_Dialog(l_Villageois2, "Mais je croyais que t'en avais plus ?"),
                                                        fn_Dialog(l_Bob, "Ah ouais c'est vrai..."),
                                                        fn_Dialog(l_Villageois2, "..."),
                                                        fn_Dialog(l_Villageois2, "Je peux vous voir ?")}));
        l_Script->mt_Add_Command(new Command_MoveOffset(l_Villageois2, {sf::Vector2f(0.0f, 1.0f)}, l_Villageois2->m_Speed, true));
        l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(0.0f, 1.0f)}, l_Player->m_Speed, true));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Player));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Villageois2));
        if (Context::smt_Get().m_Engine->mt_Get_Creature("Nivalys")->m_Gameplay_Data.m_Health > 0)
        {
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "D�sol� pour lui."),
                                                            fn_Dialog(l_Player, "Ne vous en faites pas.\nJ'ai travers� moultes �preuves plus torturantes."),
                                                            fn_Dialog(l_Player, "Je suis int�ress� par tout ce que vous\npourrez me dire sur votre agression."),
                                                            fn_Dialog(l_Villageois2, "On �tait au sanctuaire avec " + l_Villageois_Blesse->m_Name + " et on s'est fait attaqu�."),
                                                            fn_Dialog(l_Villageois2, "On a d�guerpit aussi vite que possible et on est tomb� sur vous."),
                                                            fn_Dialog(l_Villageois2, "La suite, vous la connaissez."),
                                                            fn_Dialog(l_Player, "Int�ressant."),
                                                            fn_Dialog(l_Player, "Quelqu'un peut avoir des raisons de vous en vouloir ?"),
                                                            fn_Dialog(l_Villageois2, "Non."),
                                                            fn_Dialog(l_Villageois2, "Et puis tout le monde est en ville en ce moment."),
                                                            fn_Dialog(l_Villageois2, "Qu'allez-vous faire ?"),
                                                            fn_Dialog(l_Player, "Tout semble indiqu� que la suite est\nd'aller voir ce qu'il se trame au sanctaire."),
                                                            fn_Dialog(l_Villageois2, "Vous avez raison."),
                                                            fn_Dialog(l_Villageois2, "N�anmoins, si vous trouvez de " + fn_Quest_Object("l'herbe magique") + ",\nje serais plus serein et je pourrais vous accompagner."),
                                                            fn_Dialog(l_Player, "Merci � vous."),
                                                            fn_Dialog(l_Villageois2, "Non."),
                                                            fn_Dialog(l_Villageois2, "Merci � vous.")}));

            l_Script->mt_Add_Command(new Command_MoveOffset(l_Villageois2, {sf::Vector2f(0.0f, -1.0f)}, l_Villageois2->m_Speed, true));
            l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(0.0f, 1.0f)}, l_Player->m_Speed / 2.0f, false));
            l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(-2.0f, 0.0f)}, l_Player->m_Speed / 2.0f, false));
            if ((l_Troll != nullptr) && (l_Troll->m_Gameplay_Data.m_Health > 0))
            {
                l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Il semble que les habitants de cette for�t\nsoient plus fier qu'il n'y parait."),
                                                                fn_Dialog(l_Player, "Peut-�tre que ce troll crois� pr�c�demment\npourrais nous aider.")}));
            }
        }
        else
        {
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "D�sol� pour lui."),
                                                            fn_Dialog(l_Player, "Ne vous en faites pas.\nJ'ai travers� moultes �preuves plus torturantes."),
                                                            fn_Dialog(l_Player, "J'ai r�gl� votre probl�me au sanctuaire."),
                                                            fn_Dialog(l_Villageois2, "C'et vrai ?"),
                                                            fn_Dialog(l_Villageois2, "Merci �norm�ment !"),
                                                            fn_Dialog(l_Villageois2, "Mais qu'est ce qu'il y avait ?"),
                                                            fn_Dialog(l_Player, "Un homme.\nUn mage."),
                                                            fn_Dialog(l_Player, "Il n'est plus en �tat de vous nuire.\nC'est tout ce qui compte."),
                                                            fn_Dialog(l_Villageois2, "Et bien merci � vous.")}));

            l_Script->mt_Add_Command(new Command_MoveOffset(l_Villageois2, {sf::Vector2f(0.0f, -1.0f)}, l_Villageois2->m_Speed, true));
            l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(0.0f, 1.0f)}, l_Player->m_Speed / 2.0f, false));
            l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(-2.0f, 0.0f)}, l_Player->m_Speed / 2.0f, false));
        }
        l_b_Ret = true;
    }
    else if ((m_Started == true) && (m_Villager_Seen == true) && (tgt->m_Name == l_Villageois2->m_Name))
    {
        l_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Player));
        if (l_Engine->m_Inventory.mt_Get_Item_Count("Medicinal_Herb", ItemType::Quest) > 0)
        {
            l_Engine->m_Inventory.mt_Change_Item_Count("Medicinal_Herb", -1, ItemType::Quest);
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je viens avec de " + fn_Quest_Object("l'herbe magique") + "."),
                                                            fn_Dialog(l_Villageois2, "Mon seigneur, vous �tes trop bon !")}));
            if (m_Mage_Defeated_Before_Herbe == false)
            {
                l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Je vous attend au sanctaire."),
                                                                fn_Dialog(l_Villageois2, "Ne tardez pas trop.")}));
                if (Context::smt_Get().m_Engine->mt_Get_Creature("Nivalys")->m_Gameplay_Data.m_Health <= 0)
                {
                    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "A vrai dire.\nIl n'y a plus rien � voir\nau sanctuaire."),
                                                                    fn_Dialog(l_Player, "Je me suis occup� du mage qui vous a attaqu�."),
                                                                    fn_Dialog(l_Villageois2, "Grand Direg !"),
                                                                    fn_Dialog(l_Villageois2, "C'est une excellente nouvelle !"),
                                                                    fn_Dialog(l_Villageois2, "Merci � vous �tranger !"),
                                                                    fn_Dialog(l_Player, "Ce n'est rien.\nJe n'ai fait que mon devoir.")}));
                    l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(95.0f, 102.0f), l_Player->m_Speed, false));
                    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "J'en ai fini avec cet endroit."),
                                                                     fn_Dialog(l_Player, "Je peux maintenant avancer en direction d'" + fn_City_Name("Ax�me")+ ", vers l'Est.")}));
                    m_Completed = true;
                }
                else
                {
                    l_Script->mt_Add_Command(new Command_Lights(sf::Color::White, sf::Color::Black, 0.2f));
                    l_Script->mt_Add_Command(new Command_SetPos(l_Villageois2, sf::Vector2f(69.0f, 19.0f)));
                    l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, sf::Vector2f(-1.0f, 0.0f), true));
                    l_Script->mt_Add_Command(new Command_Lights(sf::Color::Black, sf::Color::White, 0.2f));
                }
            }
            else
            {
                l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Et bien."),
                                                                fn_Dialog(l_Villageois2, "On peut dire qu'on vous doit une fi�re chandelle.")}));
                m_Completed = true;
            }
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
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "Que veux-tu ?\nTu m'a d�j� donn� de l'or."),
                                                        fn_Dialog(l_Player, "Un mal ronge cette for�t."),
                                                        fn_Dialog(l_Player, "Si personne ne s'en occupe,\nles �tres peuplant cet endroit risquent\nde conna�tre un sort sinistre."),
                                                        fn_Dialog(l_Troll, "C'est bon, je suis avec toi."),
                                                        fn_Dialog(l_Player, "C'est pourquoi je viens qu�rir ton aide\ngardien du pont de la rivi�re !"),
                                                        fn_Dialog(l_Troll, "Je t'ai dit que tu pouvais compter sur moi."),
                                                        fn_Dialog(l_Player, "Seule une �me pure ..."),
                                                        fn_Dialog(l_Troll, "STOOOOOOOOPPPP"),
                                                        fn_Dialog(l_Troll, "JE VAIS T'AIDER !!!!"),
                                                        fn_Dialog(l_Player, "..."),
                                                        fn_Dialog(l_Player, "C'est fort aimable � toi !"),
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

        m_Map_Music_Id = Context::smt_Get().m_Engine->mt_Get_Music_Id();

        l_Fight[1].push_back(CommandFightCreature());
        l_Fight[1][0].m_Tgt = l_Player;
        l_Fight[1][0].m_Logic_Id = SystemFight::ms_Human_Logic_String;
        l_Fight[1][0].m_Actions_Count = 2;

        l_Fight[2].push_back(CommandFightCreature());
        l_Fight[2][0].m_Tgt = l_Mage;
        l_Fight[2][0].m_Logic_Id = SystemFight::ms_Boss_Logic_String;
        l_Fight[2][0].m_Actions_Count = 2;

        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Mage, sf::Vector2f(-1.0f, 0.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Hein ?")}));
        l_Script->mt_Add_Command(new Command_Music("", 2.0f));
        l_Script->mt_Add_Command(new Command_Camera(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Mage->m_Pos), 2.0f, new Interpolator_Gain(2.0f), true));
        l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(67.0f, 20.0f), l_Player->m_Speed, false));
        l_Script->mt_Add_Command(new Command_Camera(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(sf::Vector2f(67.0f, 20.0f)), 2.0f, new Interpolator_Gain(2.0f), false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(-1.0f, 0.0f), true));

        if (m_Villager_Help == true)
        {
            l_Fight[1].push_back(CommandFightCreature());
            l_Fight[1].back().m_Tgt = l_Villageois2;
            l_Fight[1].back().m_Logic_Id = SystemFight::ms_Villager_Logic_String;
            l_Fight[1].back().m_Actions_Count = 2;

            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Villageois2));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Player));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Mais qui est cet homme ?"),
                                                            fn_Dialog(l_Player, "On va vite le d�couvrir !")}));
        }

        if (m_Troll_Help == true)
        {
            l_Fight[1].push_back(CommandFightCreature());
            l_Fight[1].back().m_Tgt = l_Troll;
            l_Fight[1].back().m_Logic_Id = SystemFight::ms_Villager_Logic_String;
            l_Fight[1].back().m_Actions_Count = 2;

            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Troll));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, l_Player));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "Il est comme �a depuis tout � l'heure..."),
                                                            fn_Dialog(l_Troll, "Mais que fait-il ?"),
                                                            fn_Dialog(l_Player, "On dirait qu'il incante un sort ?!")}));
        }

        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Mage));

        if ((m_Troll_Help == true) && (m_Villager_Help == true))
        {
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Villageois2, l_Troll));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, l_Villageois2));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois2, "Je ne pensais pas �tre l� avec toi.\nMais c'est une bonne chose que tu sois l�."),
                                                            fn_Dialog(l_Troll, "On verra si tu es utile.")}));
        }

        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Peut-�tre que cet homme est au courant\nde ce qui est arriv� aux villageois."),
                                                        fn_Dialog(l_Player, "J'ai quelques questions � lui poser.")}));
        l_Script->mt_Add_Command(new Command_MoveTo(l_Player, sf::Vector2f(55.0f, 20.0f), l_Player->m_Speed, false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Mage));
        if (m_Troll_Help == true)
        {
            l_Script->mt_Add_Command(new Command_MoveTo(l_Troll, sf::Vector2f(67.0f, 20.0f), l_Troll->m_Speed, false));
            l_Script->mt_Add_Command(new Command_MoveTo(l_Troll, sf::Vector2f(56.0f, 20.0f), l_Troll->m_Speed, false));
            l_Script->mt_Add_Command(new Command_MoveTo(l_Troll, sf::Vector2f(56.0f, 21.0f), l_Troll->m_Speed, false));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, l_Mage));
        }
        if (m_Villager_Help == true)
        {
            l_Script->mt_Add_Command(new Command_MoveTo(l_Villageois2, sf::Vector2f(67.0f, 20.0f), l_Villageois2->m_Speed, false));
            l_Script->mt_Add_Command(new Command_MoveTo(l_Villageois2, sf::Vector2f(56.0f, 20.0f), l_Villageois2->m_Speed, false));
            l_Script->mt_Add_Command(new Command_MoveTo(l_Villageois2, sf::Vector2f(56.0f, 19.0f), l_Villageois2->m_Speed, false));
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
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Des habitants de " + fn_City_Name("La Passe")+ "\nont �t�s attaqu� plus t�t dans la journ�e."),
                                                            fn_Dialog(l_Player, "Avez-vous vu quelque-chose ?")}));
            l_Script->mt_Add_Command(new Command_Wait(1.0f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Maintenant que vous le d�tes.")}));
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
                                                            fn_Dialog(l_Player, "Vous �tiez l� ou non ?")}));
            l_Script->mt_Add_Command(new Command_Wait(1.0f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Ecoutez, je n'ai rien � vous dire !"),
                                                            fn_Dialog(l_Player, "Vous m'avez l'air d'en savoir plus qu'il n'y parait."),
                                                            fn_Dialog(l_Player, "Que fa�tes-vous ici ?\nTout seul ?\nLa nuit ?")}));
            l_Script->mt_Add_Command(new Command_Wait(0.2f));
            l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Mage, l_Player));
            l_Script->mt_Add_Command(new Command_Wait(0.2f));
            l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Je p�che des �crevisses."),
                                                            fn_Dialog(l_Player, "C'est �a."),
                                                            fn_Dialog(l_Player, "Et moi je suis votre tante."),
                                                            fn_Dialog(l_Player, "Aller, d�tes-moi la v�rit�."),
                                                            fn_Dialog(l_Player, "C'est vous qui avez attaqu� ces pauvres villageois.")}));
        }

        l_Script->mt_Add_Command(new Command_Wait(1.5f));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Mage, "Vous n'auriez jamais d� venir ici !"),
                                                        fn_Dialog(l_Player, "Permettez-moi d'en douter.")}));
        l_Script->mt_Add_Command(new Command_MoveOffset(l_Player, {sf::Vector2f(2.0f, 0.0f)}, l_Player->m_Speed, false));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Mage));
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
    Creature* l_Player = Context::smt_Get().m_Engine->m_Player;
    Creature* l_Villageois = Context::smt_Get().m_Engine->mt_Get_Creature("Arthur");
    Creature* l_Troll = Context::smt_Get().m_Engine->mt_Get_Creature("Troll");

    l_Script->mt_Add_Command(new Command_Music("", 1.0f));
    if ((m_Villager_Help == true) && (l_Player->m_Gameplay_Data.m_Health > 0))
    {
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Villageois, "�a c'�tait pour Armand !")}));
    }
    if ((m_Troll_Help == true) && (l_Troll->m_Gameplay_Data.m_Health > 0))
    {
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Troll, l_Player));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Troll, "Je m'attendais � plus mena�ant...")}));
    }
    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Les habitants de " + fn_City_Name("La Passe")+ " ne seront plus d�rang�s\npar cet homme.")}));

    if (m_Villager_Help == true)
    {
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "J'en ai fini avec cet endroit."),
                                                        fn_Dialog(l_Player, "Je peux maintenant avancer en direction d'" + fn_City_Name("Ax�me")+ ", vers l'Est.")}));
        m_Completed = true;
    }

    if ((m_Villager_Help == false) && (m_Villager_Seen == true))
    {
        m_Mage_Defeated_Before_Herbe = true;
    }

    l_Script->mt_Add_Command(new Command_Music(m_Map_Music_Id, 1.0f));
}

void Quest_Dark_Mage::mt_Get_Description(std::vector<std::vector<sf::String>>& description)
{
    if (m_Completed == true)
    {
        description.push_back({"J'ai �lucid� le myst�re du sanctuaire et sauver les villageois !"});
    }

    if (m_Troll_Asked == true)
    {
        description.push_back({"Le troll m'attend au sanctuaire\npour r�gler cette affaire au clair"});
    }

    if (m_Villager_Help == true)
    {
        description.push_back({"J'ai trouv� la plante.",
                              "Je dois retrouver le villageois au sanctuaire"});
    }
    else
    {
        if (m_Villager_Seen == true)
        {
            description.push_back({"Le villageois pas bless� m'a propos� son aide.",
                                  "Je doit tout d'abord trouver de une plante pour leur herboriste."});
        }

        if ((m_Troll_Asked == false) && (Context::smt_Get().m_Engine->mt_Get_Creature("Troll")->m_Gameplay_Data.m_Health > 0))
        {
            description.push_back({"Le troll pourrait peut-�tre m'aider �galement ?"});
        }
    }

    if (m_Started == true)
    {
        description.push_back({"Des villageois se sont fait attaqu�\nalors qu'ils �taient au sanctuaire."});
    }

}






Quest_Tutoriel::Quest_Tutoriel()
{
    m_Quest_Name = "Tutoriel";

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
    Creature* l_Wolf = Context::smt_Get().m_Engine->mt_Get_Creature("Wolf");
    sf::Color l_Player_Color(sf::Color::Cyan);

    if ((tgt->m_Name == "Tutoriel_Wolf_Warn") && (m_Wolf_Seen == false))
    {
        l_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Attention au loup devant moi.")}));
        l_Script->mt_Add_Command(new Command_Camera(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Wolf->m_Pos), 1.0f, new Interpolator_Gain(2.0f), true));
        l_Script->mt_Add_Command(new Command_Wait(1.0f));
        l_Script->mt_Add_Command(new Command_Camera(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(l_Player->m_Pos), 1.0f, new Interpolator_Gain(2.0f), false));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Ca va �tre mon premier combat depuis longtemps."),
                                  fn_Dialog(l_Player, "Je dois bien me d�cider � attaquer\nlorqu'il est � port�."),
                                  fn_Dialog(l_Player, "Je peux �galement me d�placer pour\nm'approcher de lui\nou au contraire m'en �loigner."),
                                  fn_Dialog(l_Player, "Dans tous les cas, j'ai l'avantage\nde d�cider en premier\nde ce que je vais faire."),}));
        l_Script->mt_Add_Command(new Command_ShowDialog({"~Les combats se jouent au tour par tour.~",
                                                        "~Attaquer fait terminer le tour une fois l'attaque r�alis�e.~",
                                                        "~Passer permet de laisser le prochain personnage jouer.~",
                                                        "~Toute autre action permet d'en r�aliser\nune second dans la foul�e.~",
                                                        "~Il est possible d'arr�ter un d�placement sans avoir consomm�\ntoute la distance disponible.\nC'est que la touche " + fn_Command_Key("Espace") +" permet de faire.~",
                                                        "~Les " + fn_Command_Key("cercles") + " d�limitent la distance\nd'une attaque ou d'un d�placement.~",
                                                        "~Le " + fn_Command_Key("carr� rouge") + " est la cible actuelle de l'attaque.~",
                                                        "~Le choix de la cible se fait\nen utilisant " + fn_Command_Key("les fl�ches directionnelles") + ".~",
                                                        "~La touche " + fn_Command_Key("Echap") + " permet,\ndans certaines conditions, de revenir en arri�re.~",
                                                        "~Attention � bien choisir la cible.~"}));
        m_Wolf_Seen = true;
    }
    else if (((tgt->m_Name == "Wolf") || (tgt->m_Name == "Tutoriel_Wolf_Fight")) && (m_Wolf_Defeated == false))
    {
        std::map<int, std::vector<CommandFightCreature>> l_Parties;
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

        Context::smt_Get().m_System_Sound.mt_Play_Sound("Wolf",
                                                        sf::Vector3f(l_Wolf->m_Pos.x - l_Engine->m_Player->m_Pos.x,
                                                                     0.0f,
                                                                     l_Wolf->m_Pos.y - l_Engine->m_Player->m_Pos.y),
                                                        true);

        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Wolf, l_Player));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, l_Wolf));
        l_Script->mt_Add_Command(new Command_Fight(l_Parties, std::bind(Quest_Tutoriel::mt_On_Victory, this), "Fight_Wolf"));
        m_Wolf_Defeated = true;
        l_Ret = true;
    }
    else if ((tgt->m_Name == "Tutoriel_Chest_Warn") && (m_Chest_Seen == false))
    {
        l_Engine->m_Player->m_Desired_Vel = {0.0f, 0.0f};
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Vers l'Est se trouve " + fn_City_Name("Ax�me")+ ",\nma destination.")}));
        l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Engine->m_Player, sf::Vector2f(0.0f, 1.0f), true));
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je devrais tout de m�me regarder o� m�ne ce chemin au Sud."),
                                                         fn_Dialog(l_Player, "Histoire d'�tre sur de ne rien rater.")}));
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
    int l_Life_Count = 8;
    int l_Psy_Count = 4;

    l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Ah !"),
                                                    fn_Dialog(l_Player, "La sale b�te !")}));
    l_Script->mt_Add_Command(new Command_Creature_LookAt(l_Player, sf::Vector2f(0.0f, 1.0f), true));

    if (    (l_Player->m_Gameplay_Data.m_Health != l_Player->m_Gameplay_Data.m_Health_Max)
        ||  (l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy != l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max))
    {
        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "Je ferais bien de me soigner\navant de tomber sur une autre bestiole.")}));
        fn_Command_Give_Items(l_Script, {ChestData("LifePotion", ItemType::Edible, l_Life_Count)});
        fn_Command_Give_Items(l_Script, {ChestData("PsyPotion", ItemType::Edible, l_Psy_Count)});
        l_Script->mt_Add_Command(new Command_ShowDialog({"~La touche " + fn_Command_Key("I") + " permet d'acc�der � l'inventaire.~",
                                                        "~Une fois dans l'inventaire, la navigation se fait avec\nles " + fn_Command_Key("touches directionnelles") + ".~",
                                                        "~Pour s�lectionner un menu ou un item, appuyez sur " + fn_Command_Key("Espace") + ".~"}));

        l_Script->mt_Add_Command(new Command_ShowDialog({fn_Dialog(l_Player, "La prochaine fois, je devrais utiliser\nma comp�tence sp�ciale pour en finir plus vite.")}));
        l_Script->mt_Add_Command(new Command_AddSkill(l_Player, Context::smt_Get().m_Engine->m_Player_Skill_Id));
    }
}

bool Quest_Tutoriel::mt_Populate_Dynamics(std::vector<Resource<Dynamic>>& dyns, const std::string& map_id)
{
    return true;
}

void Quest_Tutoriel::mt_Get_Description(std::vector<std::vector<sf::String>>& description)
{
    if (m_Completed == true)
    {
        description.push_back({"J'ai fini cette qu�te !"});
    }

    if (m_Chest_Open == true)
    {
        description.push_back({"J'ai trouv� un coffre au bout du chemin\net une pi�ce d'or � l'int�rieur.",
                              "Je me demande � quoi elle pourra me servir."});
    }
    else if (m_Chest_Seen == true)
    {
        description.push_back({"Je ferais bien d'aller voir ce qu'il se trouve\nau bout de ce chemin au sud de la montagne."});
    }

    if (m_Wolf_Defeated == true)
    {
        description.push_back({"Le loup n'est plus un probl�me."});
    }
    else if (m_Wolf_Seen == true)
    {
        description.push_back({"Un loup se trouve en travers de mon chemin.",
                              "Je peux essayer de passer � c�t�.",
                              "Sait-on jamais, �a peut marcher ?"});
    }
}

