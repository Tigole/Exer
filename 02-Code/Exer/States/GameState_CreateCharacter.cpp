#include "GameState_CreateCharacter.hpp"
#include "../GameEngine.hpp"


void GameState_CreateCharacter::mt_Create(void)
{
    ClassData l_Data;

    l_Data.m_Class_Name = "Paladin";
    l_Data.m_Class_Skill = "ThunderSword";
    l_Data.m_Class_Description = "Le Paladin manie l'épée avec grâce et volupté.";
    l_Data.m_Text_Color = sf::Color::Yellow;
    l_Data.m_Sprite = Context::smt_Get().m_SpriteSheetAnimators.mt_Get_Resource("_Theo");
    l_Data.m_GameplayData.m_Health_Max = 18;
    l_Data.m_GameplayData.m_Data_Aventure.m_Psy_Max = 6;
    m_Classes.push_back(l_Data);

    l_Data.m_Class_Name = "Lutteur";
    l_Data.m_Class_Skill = "EarthFist";
    l_Data.m_Class_Description = "Le lutteur affronte ses adversaires\nen utilisant sa puissance physique.";
    l_Data.m_Text_Color = sf::Color(160, 82, 45);
    l_Data.m_Sprite = Context::smt_Get().m_SpriteSheetAnimators.mt_Get_Resource("_Krayn");
    l_Data.m_GameplayData.m_Health_Max = 16;
    l_Data.m_GameplayData.m_Data_Aventure.m_Psy_Max = 8;
    m_Classes.push_back(l_Data);

    l_Data.m_Class_Name = "Archer";
    l_Data.m_Class_Skill = "IceArrow";
    l_Data.m_Class_Description = "l'archer préfère attaquer ses ennemis à distance.\nIl ne supporte pas bien le contact.";
    l_Data.m_Text_Color = sf::Color(100, 149, 237);
    l_Data.m_Sprite = Context::smt_Get().m_SpriteSheetAnimators.mt_Get_Resource("_Shin");
    l_Data.m_GameplayData.m_Health_Max = 14;
    l_Data.m_GameplayData.m_Data_Aventure.m_Psy_Max = 10;
    m_Classes.push_back(l_Data);

    l_Data.m_Class_Name = "Mage";
    l_Data.m_Class_Skill = "FireBall";
    l_Data.m_Class_Description = "La mage lance des boules de feu sur ses ennemis.";
    l_Data.m_Text_Color = sf::Color::Red;
    l_Data.m_Sprite = Context::smt_Get().m_SpriteSheetAnimators.mt_Get_Resource("_Bob");
    l_Data.m_GameplayData.m_Health_Max = 11;
    l_Data.m_GameplayData.m_Data_Aventure.m_Psy_Max = 13;
    m_Classes.push_back(l_Data);

    m_Selected_Class = 0;
    m_Name_Set = false;

    m_Font = Context::smt_Get().m_Fonts.mt_Get_Resource("BOOKOS");
}

void GameState_CreateCharacter::mt_Destroy(void)
{
    Context::smt_Get().m_Fonts.mt_Release(m_Font);
}

void GameState_CreateCharacter::mt_Handle_Event(sf::Event& event)
{
    EventType l_Type = fn_EventType(event);

    if (m_Name_Set == false)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == '\n' || event.text.unicode == '\r')
            {
                m_Name_Set = true;
            }
            else if (event.text.unicode == '\b')
            {
                if (m_Player_Name.getSize() != 0)
                {
                    m_Player_Name = m_Player_Name.substring(0, m_Player_Name.getSize() - 1);
                }
            }
            else
            {
                m_Player_Name += event.text.unicode;
            }
        }
    }
    else
    {
        Creature* l_Player = Context::smt_Get().m_Engine->m_Player;

        switch(l_Type)
        {
        case EventType::Released_Right:
            m_Selected_Class++;
            if (m_Selected_Class >= m_Classes.size())
            {
                m_Selected_Class = m_Classes.size() - 1;
            }
            break;
        case EventType::Released_Left:
            m_Selected_Class--;
            if (m_Selected_Class < 0)
            {
                m_Selected_Class = 0;
            }
            break;
        case EventType::Validate:
            l_Player->m_Gameplay_Data = m_Classes[m_Selected_Class].m_GameplayData;
            l_Player->m_Gameplay_Data.m_Health = l_Player->m_Gameplay_Data.m_Health_Max;
            l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy = l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max;
            l_Player->m_Gameplay_Data.m_Skills.push_back(Context::smt_Get().m_Skills.mt_Get_Resource("Hit"));
            l_Player->m_Name = m_Player_Name;
            l_Player->mt_Set_Sprite(*(m_Classes[m_Selected_Class].m_Sprite.m_Resource_Id));
            Context::smt_Get().m_Engine->m_State = GameStateType::Game;

            break;
        case EventType::Cancel:
            m_Name_Set = false;
            m_Player_Name.clear();
            break;
        default:
            break;
        }
    }
}

void GameState_CreateCharacter::mt_Update(float delta_time_s)
{
    if (m_Name_Set == false)
    {
        //
    }
    else
    {
        for (std::size_t ii = 0; ii < m_Classes.size(); ii++)
        {
            m_Classes[ii].m_Sprite_Rect = m_Classes[ii].m_Sprite->mt_Get_Rect(delta_time_s, sf::Vector2f(0.0f, 1.0f), ii == m_Selected_Class, m_Classes[ii].m_Sprite_Data);
        }
    }
}

void GameState_CreateCharacter::mt_Draw(sf::RenderTarget& target)
{
    sf::Text l_Text;
    sf::Sprite l_Sprite;
    sf::RectangleShape l_Rect;
    float l_x_Offset(0.0f);

    l_Text.setFont(*m_Font);

    if (m_Name_Set == false)
    {
        l_Text.setCharacterSize(40);
        l_Text.setString("Quel est ton nom aventurier ?");
        l_Text.setOrigin(l_Text.getLocalBounds().width / 2, l_Text.getLocalBounds().height / 2);
        l_Text.setPosition(target.getSize().x / 2, 60.0f);
        target.draw(l_Text);

        l_Text.setCharacterSize(20);
        l_Text.setString("Valide ton nom avec \"Entrée\".");
        l_Text.setOrigin(l_Text.getLocalBounds().width / 2, l_Text.getLocalBounds().height / 2);
        l_Text.setPosition(target.getSize().x / 2, 100.0f);
        target.draw(l_Text);

        l_Text.setCharacterSize(40);
        l_Text.setString("\"" + m_Player_Name + "\"");
        l_Text.setOrigin(l_Text.getLocalBounds().width / 2, l_Text.getLocalBounds().height / 2);
        l_Text.setPosition(target.getSize().x / 2, target.getSize().y / 2);
        target.draw(l_Text);
    }
    else
    {
        l_Text.setCharacterSize(40);
        l_Text.setString("Quelle classe veux-tu jouer ?");
        l_Text.setOrigin(l_Text.getLocalBounds().width / 2, l_Text.getLocalBounds().height / 2);
        l_Text.setPosition(target.getSize().x / 2, 60.0f);
        target.draw(l_Text);

        l_Text.setCharacterSize(20);
        l_Text.setString("Utilise les \"flèches\" pour sélectionner et \"Espace\" pour valider");
        l_Text.setOrigin(l_Text.getLocalBounds().width / 2, l_Text.getLocalBounds().height / 2);
        l_Text.setPosition(target.getSize().x / 2, 100.0f);
        target.draw(l_Text);


        l_Text.setCharacterSize(20);
        l_Rect.setFillColor(sf::Color::Transparent);
        l_Rect.setOutlineColor(sf::Color::Blue);
        l_Rect.setOutlineThickness(2.0f);
        l_Rect.setSize(sf::Vector2f(target.getSize().x / 2, target.getSize().y / 2));
        l_Rect.setOrigin(l_Rect.getSize().x / 2, l_Rect.getSize().y / 2);
        l_Rect.setPosition(target.getSize().x / 2, target.getSize().y / 2);

        //target.draw(l_Rect);

        l_x_Offset = l_Rect.getSize().x / (m_Classes.size() + 1);

        for (std::size_t ii = 0; ii < m_Classes.size(); ii++)
        {
            ClassData& l_Data = m_Classes[ii];

            l_Sprite.setTexture(l_Data.m_Sprite->mt_Get_Texture());
            l_Sprite.setTextureRect(l_Data.m_Sprite_Rect);
            l_Sprite.setOrigin(l_Sprite.getLocalBounds().width / 2, l_Sprite.getLocalBounds().height / 2);
            l_Sprite.setPosition(target.getSize().x / 2, target.getSize().y / 2);
            l_Sprite.setPosition(l_Rect.getPosition().x - l_Rect.getLocalBounds().width / 2 + (ii + 1) * l_x_Offset, target.getSize().y / 2);
            l_Sprite.setScale(sf::Vector2f(1.5f, 1.5f));

            l_Text.setFillColor(l_Data.m_Text_Color);
            l_Text.setString(l_Data.m_Class_Name);
            l_Text.setOrigin(l_Text.getLocalBounds().width / 2, l_Text.getLocalBounds().height / 2);
            l_Text.setPosition(l_Sprite.getPosition() + sf::Vector2f(0.0f, l_Sprite.getGlobalBounds().height));

            target.draw(l_Text);
            target.draw(l_Sprite);
        }

        l_Rect.setOutlineColor(sf::Color::Magenta);
        l_Rect.setSize(sf::Vector2f(target.getSize().x / 2, target.getSize().y / 4));
        l_Rect.setOrigin(l_Rect.getSize().x / 2, l_Rect.getSize().y / 2);
        l_Rect.setPosition(target.getSize().x / 2, target.getSize().y / 2);

        //target.draw(l_Rect);

        l_Text.setFillColor(sf::Color::White);
        l_Text.setString(m_Classes[m_Selected_Class].m_Class_Description);
        l_Text.setOrigin(l_Text.getLocalBounds().width / 2, l_Text.getLocalBounds().height / 2);

        l_Rect.setOutlineColor(sf::Color::Yellow);
        l_Rect.setSize(sf::Vector2f(target.getSize().x / 2, target.getSize().y / 4));
        l_Rect.setOrigin(l_Rect.getSize().x / 2, l_Rect.getSize().y / 2);
        l_Rect.setPosition(target.getSize().x / 2, target.getSize().y * 3.0f / 4.0f);
        l_Text.setPosition(l_Rect.getPosition());

        //target.draw(l_Rect);
        target.draw(l_Text);
    }
}
