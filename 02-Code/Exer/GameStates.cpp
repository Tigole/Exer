#include "GameStates.hpp"
#include "Context.hpp"
#include "GameEngine.hpp"

#include "RichText.hpp"

GameStateDisplayer::GameStateDisplayer()
{
    m_Gradient_Period_s = 3.0f;
}

void GameStateDisplayer::mt_Update(float delta_time_s)
{
    m_Accumulated_Time_s += delta_time_s;

    m_Selected_Color = fn_Interpolate(&m_Interpolator, m_Accumulated_Time_s, m_Gradient_Period_s, m_Color_Theme[1], m_Color_Theme[0]);
}


/*void GameState_Quest::mt_On_Entry()
{
    //
}*/

void GameState_Quest::mt_Create(void)
{
    m_Selected_Quest = -1;
    m_Font = Context::smt_Get().m_Fonts.mt_Get_Resource("BOOKOS");

    m_Color_Theme[0] = sf::Color(0x2e, 0x00, 0x6c);
    m_Color_Theme[1] = sf::Color(0x6f, 0x00, 0xff);
}

void GameState_Quest::mt_Destroy(void)
{
    Context::smt_Get().m_Fonts.mt_Release(m_Font);
}

void GameState_Quest::mt_Handle_Event(sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            Context::smt_Get().m_Engine->m_State = GameStateType::Game;
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Cancel_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Left)
        {
            m_Selected_Quest--;
            if (m_Selected_Quest < 0)
            {
                m_Selected_Quest = 0;
            }
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            m_Selected_Quest++;
            if (m_Selected_Quest >= Context::smt_Get().m_Engine->m_System_Quest.m_Quests.size())
            {
                m_Selected_Quest = Context::smt_Get().m_Engine->m_System_Quest.m_Quests.size() - 1;
            }
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Up)
        {
            m_Selected_Quest -= 3;
            if (m_Selected_Quest < 0)
            {
                m_Selected_Quest = 0;
            }
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            m_Selected_Quest += 3;
            if (m_Selected_Quest >= Context::smt_Get().m_Engine->m_System_Quest.m_Quests.size())
            {
                m_Selected_Quest = Context::smt_Get().m_Engine->m_System_Quest.m_Quests.size();
            }
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
    }
}

void GameState_Quest::mt_Update(float delta_time_s)
{
    if ((m_Selected_Quest < 0) && (Context::smt_Get().m_Engine->m_System_Quest.m_Quests.size() != 0))
    {
        m_Selected_Quest = 0;
    }
    GameStateDisplayer::mt_Update(delta_time_s);
}

void GameState_Quest::mt_Draw(sf::RenderTarget& target)
{
    sf::Vector2f l_Pos;
    sfe::RichText l_Text;
    sf::CircleShape l_Cursor;
    int l_Cnt(0);
    sf::RectangleShape l_Rect;
    sf::FloatRect l_Bounds;
    sf::Font* l_Font = Context::smt_Get().m_Dialog->m_Dialog_Font.m_Resource;
    SystemQuest* l_System_Quest = &Context::smt_Get().m_Engine->m_System_Quest;
    auto l_fn_Draw_Title = [&](const sf::FloatRect& bounds)
    {
        sf::Vector2f l_Offset;

        l_Rect.setFillColor(m_Color_Theme[1]);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Text.setCharacterSize(40);
        l_Text.setString("Quêtes");
        l_Text.setOrigin(l_Text.getGlobalBounds().width / 2.0f, 0.0f);
        l_Text.setPosition(bounds.left + bounds.width / 2.0f, bounds.top + m_Margin);
        target.draw(l_Text);
    };
    auto l_fn_Draw_Quests = [&](const sf::FloatRect& bounds)
    {
        sf::Vector2f l_Offset;

        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Offset.x = bounds.width / 3.0f - m_Margin * 2.0f;
        l_Offset.y = bounds.height / 2.0f - m_Margin * 2.0f;

        l_Pos.x = bounds.left + m_Margin;
        l_Pos.y = bounds.top + m_Margin;
        l_Text.setCharacterSize(25);

        l_Rect.setSize({l_Offset.x, l_Offset.y});

        for (int ii = 0; ii < l_System_Quest->m_Quests.size(); ii++)
        {
            Quest* l_Quest = l_System_Quest->m_Quests[ii].m_Resource;
            l_Rect.setPosition(l_Pos);

            l_Text.setString(l_Quest->m_Quest_Name);
            l_Text.setOrigin(l_Text.getGlobalBounds().width / 2.0f, 0.0f);
            l_Text.setPosition(l_Rect.getGlobalBounds().left + l_Rect.getGlobalBounds().width / 2.0f , l_Pos.y);

            if (ii == m_Selected_Quest)
            {
                l_Rect.setFillColor(m_Selected_Color);
            }
            else
            {
                l_Rect.setFillColor(m_Inactive_Rect_Filling_Color);
            }

            target.draw(l_Rect);
            target.draw(l_Text);

            l_Pos.x += l_Offset.x + m_Margin * 2.0f;
            if (l_Pos.x >= bounds.left + bounds.width)
            {
                l_Pos.x = bounds.left + m_Margin;
                l_Pos.y += l_Offset.y + m_Margin * 2.0f;
            }
        }

        l_Text.setOrigin(0.0f, 0.0f);
    };
    auto l_fn_Draw_Description = [&](const sf::FloatRect& bounds)
    {
        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Pos.x = bounds.left;
        l_Pos.y = bounds.top + m_Margin;
        l_Text.setCharacterSize(15);

        if (m_Selected_Quest < l_System_Quest->m_Quests.size())
        {
            Quest* l_Quest = l_System_Quest->m_Quests[m_Selected_Quest].m_Resource;
            std::vector<std::vector<sf::String>> l_Description;

            l_Quest->mt_Get_Description(l_Description);

            l_Pos.x += 3.0f;

            for (std::size_t paragraph = 0; paragraph < l_Description.size(); paragraph++)
            {
                for (std::size_t line = 0; line < l_Description[paragraph].size(); line++)
                {
                    l_Text.setPosition(l_Pos);
                    l_Text.setString(l_Description[paragraph][line]);
                    target.draw(l_Text);
                    l_Pos.y += l_Text.getGlobalBounds().height + 10.0f;
                }

                l_Pos.y += 10.0f;
            }
        }
    };

    l_Text.setFont(*l_Font);
    l_Cursor.setFillColor(sf::Color::White);
    l_Cursor.setPointCount(3);
    l_Cursor.setRadius(5.0f);

    l_Bounds.left = m_Margin;
    l_Bounds.width = target.getSize().x - 2 * l_Bounds.left;
    l_Bounds.top = m_Margin;
    l_Bounds.height = 75.0f - m_Margin;

    l_fn_Draw_Title(l_Bounds);

    l_Bounds.top += l_Bounds.height + 2*m_Margin;
    l_Bounds.height = 150.0f - m_Margin;
    l_fn_Draw_Quests(l_Bounds);

    auto l_old_y = l_Bounds.top + l_Bounds.height;

    l_Bounds.top += l_Bounds.height + 2*m_Margin;
    l_Bounds.height = target.getSize().y - l_Bounds.top - m_Margin;
    l_fn_Draw_Description(l_Bounds);
}




/*void GameState_Item::mt_On_Entry()
{
    //
}*/


void GameState_Item::mt_Create(void)
{
    m_Item_Type = ItemType::Edible;
    m_Select_Item_Type = true;

    m_Color_Theme[0] = sf::Color(0x85, 0x23, 0x00);
    m_Color_Theme[1] = sf::Color(0xff, 0x45, 0x00);
}

void GameState_Item::mt_Destroy(void)
{}

void GameState_Item::mt_Handle_Event(sf::Event& event)
{
    EventType l_Type;

    l_Type = fn_EventType(event);

    if ((m_Select_Item_Type == true) && (m_Type_Locked == false))
    {
        if (l_Type == EventType::Cancel)
        {
            Context::smt_Get().m_Engine->m_State = GameStateType::Game;
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Cancel_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (l_Type == EventType::Validate)
        {
            if (Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item_Count(m_Item_Type) > 0)
            {
                m_Select_Item_Type = false;
                Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
            }
            else
            {
                Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Buzz_String, {0.0f, 0.0f, 0.0f}, true);
            }
        }
        if ((l_Type == EventType::Released_Left) || (l_Type == EventType::Released_Up))
        {
            --(int&)m_Item_Type;
            if ((int)m_Item_Type < 0)
                m_Item_Type = (ItemType)0;
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);

            std::cout << (int)m_Item_Type << '\n';
        }
        else if ((l_Type == EventType::Released_Right) || (l_Type == EventType::Released_Down))
        {
            ++(int&)m_Item_Type;
            if (m_Item_Type >= ItemType::COUNT)
                m_Item_Type = (ItemType)((int)ItemType::COUNT - 1);
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);

            std::cout << (int)m_Item_Type << '\n';
        }
    }
    else
    {
        if (l_Type == EventType::Cancel)
        {
            m_Select_Item_Type = true;
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Cancel_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (l_Type == EventType::Validate)
        {
            Item* l_Item = Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item<Item>(m_Current_Item[m_Item_Type], m_Item_Type);

            if (l_Item != nullptr)
            {
                if (m_Item_Type == ItemType::Edible)
                {
                    Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
                    Context::smt_Get().m_Engine->m_Script.mt_Add_Command(new Command_Fight_Use_Object(l_Item, Context::smt_Get().m_Engine->m_Player));
                    Context::smt_Get().m_Engine->m_State = GameStateType::Game;
                }
                else
                {
                    Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Buzz_String, {0.0f, 0.0f, 0.0f}, true);
                }
            }

        }
        if ((l_Type == EventType::Released_Left) || (l_Type == EventType::Released_Up))
        {
            m_Current_Item[m_Item_Type]--;
            if (m_Current_Item[m_Item_Type] < 0)
            {
                m_Current_Item[m_Item_Type] = 0;
            }
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if ((l_Type == EventType::Released_Right) || (l_Type == EventType::Released_Down))
        {
            m_Current_Item[m_Item_Type]++;
            if (m_Current_Item[m_Item_Type] >= Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item_Count(m_Item_Type))
            {
                m_Current_Item[m_Item_Type] = Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item_Count(m_Item_Type) - 1;
            }
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
    }
}

void GameState_Item::mt_Update(float delta_time_s)
{
    GameStateDisplayer::mt_Update(delta_time_s);
}

void GameState_Item::mt_Draw(sf::RenderTarget& target)
{
    SystemInventory* l_Inventory = &Context::smt_Get().m_Engine->m_Inventory;
    std::size_t ii = 0;
    std::size_t l_size = l_Inventory->mt_Get_Item_Count(m_Item_Type);
    sf::Font* l_Font = Context::smt_Get().m_Dialog->m_Dialog_Font.m_Resource;
    sf::Text l_Text;
    sf::Vector2f l_Pos;
    sf::CircleShape l_Cursor;
    sf::Vector2f l_Sprite_Size(32.0f, 32.0f);
    sf::RectangleShape l_Rect;
    sf::FloatRect l_Bounds;
    auto l_fn_ItemType_To_String = [&](ItemType type)
    {
        sf::String l_Ret;

        switch(type)
        {
        case ItemType::Edible:
            l_Ret = "Consommables";
            break;
        case ItemType::Equipment:
            l_Ret = "Equipements";
            break;
        case ItemType::Quest:
            l_Ret = "Quêtes";
            break;
        default:
            l_Ret = "???";
            break;
        }

        return l_Ret;
    };
    auto l_fn_Draw_Title = [&](const sf::FloatRect& bounds)
    {
        sf::Vector2f l_Offset;

        l_Rect.setFillColor(m_Color_Theme[1]);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Text.setCharacterSize(40);
        l_Text.setString("Inventaire");
        l_Text.setFillColor(sf::Color::White);
        l_Text.setOrigin(l_Text.getGlobalBounds().width / 2.0f, 0.0f);
        l_Text.setPosition(bounds.left + bounds.width / 2.0f, bounds.top + m_Margin);
        target.draw(l_Text);
    };
    auto l_fn_Draw_Types = [&](const sf::FloatRect& bounds)
    {
        sf::Vector2f l_Offset;

        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Offset.x = bounds.width / 3.0f - m_Margin * 2.0f;
        l_Offset.y = bounds.height / 2.0f - m_Margin * 2.0f;

        l_Pos.x = bounds.left + m_Margin;
        l_Pos.y = bounds.top + m_Margin;
        l_Text.setFillColor(sf::Color::White);
        l_Text.setCharacterSize(25);

        l_Rect.setSize({l_Offset.x, l_Offset.y});

        for (int ii = 0; ii < (int)ItemType::COUNT; ii++)
        {
            l_Rect.setPosition(l_Pos);

            l_Text.setString(l_fn_ItemType_To_String((ItemType)ii));
            l_Text.setOrigin(l_Text.getGlobalBounds().width / 2.0f, 0.0f);
            l_Text.setPosition(l_Rect.getGlobalBounds().left + l_Rect.getGlobalBounds().width / 2.0f , l_Pos.y);

            if ((ItemType)ii == m_Item_Type)
            {
                l_Rect.setFillColor(m_Selected_Color);
            }
            else
            {
                l_Rect.setFillColor(m_Inactive_Rect_Filling_Color);
            }

            target.draw(l_Rect);
            target.draw(l_Text);

            l_Pos.x += l_Offset.x + m_Margin * 2.0f;
            if (l_Pos.x >= bounds.left + bounds.width)
            {
                l_Pos.x = bounds.left + m_Margin;
                l_Pos.y += l_Offset.y + m_Margin * 2.0f;
            }
        }

        l_Text.setOrigin(0.0f, 0.0f);
    };
    auto l_fn_Draw_Items = [&](const sf::FloatRect& bounds)
    {
        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Text.setStyle(sf::Text::Regular);
        l_Text.setCharacterSize(15);
        l_Text.setFillColor(sf::Color(192, 192, 192));
        l_Pos.x = bounds.left + m_Margin;
        l_Pos.y = bounds.top + m_Margin;
        for (; ii < l_size; ii++)
        {
            Item* l_Item = l_Inventory->mt_Get_Item<Item>(ii, m_Item_Type);

            if (l_Item->m_Sprite.m_Resource != nullptr)
            {
                sf::RectangleShape l_Sprite;
                sf::FloatRect l_Global_Bounds;

                l_Sprite.setTexture(&l_Item->m_Sprite->mt_Get_Texture());
                l_Sprite.setTextureRect(l_Item->m_Sprite->mt_Get_Rect(l_Item->m_Sprite_Rect.x, l_Item->m_Sprite_Rect.y));
                l_Sprite.setSize(l_Sprite_Size);
                l_Sprite.setPosition(l_Pos);
                l_Sprite.setFillColor(sf::Color::White);

                if (m_Current_Item[m_Item_Type] == ii)
                {
                    l_Sprite.setOutlineColor(sf::Color::White);
                    l_Sprite.setOutlineThickness(2.0f);
                    if (m_Select_Item_Type == false)
                    {
                        l_Sprite.setOutlineColor(sf::Color(255, 255, 255, 255 * (0.5 * sinf(m_Accumulated_Time_s * 2.0f * 3.14f * 0.5f) + 0.5f)));
                    }
                }
                l_Global_Bounds = l_Sprite.getGlobalBounds();

                l_Text.setString(std::to_string(l_Item->m_Quantity));
                l_Text.setPosition(l_Global_Bounds.left, l_Global_Bounds.top);

                target.draw(l_Sprite);
                target.draw(l_Text);

                l_Pos.x += l_Sprite_Size.x + 2*m_Margin;
                if ((l_Pos.x + m_Margin) >= bounds.left + bounds.width)
                {
                    l_Pos.x = bounds.left + m_Margin;
                    l_Pos.y += l_Sprite_Size.y + m_Margin*2;
                }
            }
            else
            {
                l_Text.setPosition(l_Pos);
                l_Text.setString(l_Item->m_Name);

                target.draw(l_Text);

                if (m_Current_Item[m_Item_Type] == ii)
                {
                    l_Cursor.setPosition(l_Pos - sf::Vector2f(m_Margin, 0.0f));
                    target.draw(l_Cursor);
                }

                l_Pos.y += l_Text.getGlobalBounds().height + 2*m_Margin;
            }
        }



    };
    auto l_fn_Draw_Description = [&](const sf::FloatRect& bounds)
    {
        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Pos.y = bounds.top + m_Margin;

        if (m_Current_Item[m_Item_Type] < l_Inventory->mt_Get_Item_Count(m_Item_Type))
        {
            l_Text.setColor(sf::Color::White);

            l_Pos.x += 3.0f;
            l_Text.setCharacterSize(25);
            l_Text.setPosition(l_Pos);
            l_Text.setString(l_Inventory->mt_Get_Item<Item>(m_Current_Item[m_Item_Type], m_Item_Type)->m_Name);
            target.draw(l_Text);
            l_Pos.y += l_Text.getGlobalBounds().height + 10.0f;
            l_Text.setCharacterSize(20);
            l_Text.setPosition(l_Pos);
            l_Text.setString(l_Inventory->mt_Get_Item<Item>(m_Current_Item[m_Item_Type], m_Item_Type)->m_Description);
            target.draw(l_Text);
        }
    };

    l_Text.setFont(*l_Font);
    l_Cursor.setFillColor(sf::Color::White);
    l_Cursor.setPointCount(3);
    l_Cursor.setRadius(5.0f);

    l_Bounds.left = m_Margin;
    l_Bounds.width = target.getSize().x - 2 * l_Bounds.left;
    l_Bounds.top = m_Margin;
    l_Bounds.height = 75.0f - m_Margin;

    l_fn_Draw_Title(l_Bounds);

    l_Bounds.top += l_Bounds.height + 2*m_Margin;
    l_Bounds.height = 150.0f - m_Margin;
    l_fn_Draw_Types(l_Bounds);

    auto l_old_y = l_Bounds.top + l_Bounds.height;


    l_Bounds.top = target.getSize().y - 150.0f;
    l_Bounds.height = target.getSize().y - l_Bounds.top - m_Margin;
    l_fn_Draw_Description(l_Bounds);

    l_Bounds.height = l_Bounds.top - 2*m_Margin - (l_old_y + 2*m_Margin);
    l_Bounds.top = l_old_y + 2*m_Margin;
    l_fn_Draw_Items(l_Bounds);
}


void GameState_Item::mt_Lock_Type(ItemType item_type)
{
    if (item_type == ItemType::COUNT)
    {
        m_Select_Item_Type = true;
        m_Type_Locked = false;
    }
    else
    {
        m_Select_Item_Type = false;
        m_Type_Locked = true;
        m_Item_Type = item_type;
        m_Current_Item[m_Item_Type] = 0;
    }
}

Item* GameState_Item::mt_Get_Selected_Item(void)
{
    Item* l_Ret(nullptr);
    auto l_it = m_Current_Item.find(m_Item_Type);

    if (l_it != m_Current_Item.end())
    {
        l_Ret = Context::smt_Get().m_Engine->m_Inventory.mt_Get_Item<Item>(l_it->second, m_Item_Type);
    }

    return l_Ret;
}


/*void GameState_Skill::mt_On_Entry()
{
    //
}*/


void GameState_Skill::mt_Create(void)
{
    m_Selected_Skill = 0;
    m_Font = Context::smt_Get().m_Fonts.mt_Get_Resource("BOOKOS");


    m_Color_Theme[0] = sf::Color(0x80, 0x00, 0x00);
    m_Color_Theme[1] = sf::Color(0xaf, 0x00, 0x00);
}

void GameState_Skill::mt_Destroy(void)
{
    Context::smt_Get().m_Fonts.mt_Release(m_Font);
}

void GameState_Skill::mt_Handle_Event(sf::Event& event)
{
    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            Context::smt_Get().m_Engine->m_State = GameStateType::Game;
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Cancel_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Left)
        {
            m_Selected_Skill--;
            if (m_Selected_Skill < 0)
                m_Selected_Skill = 0;
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            m_Selected_Skill++;
            if (m_Selected_Skill >= Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills.size())
            {
                m_Selected_Skill = Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills.size() - 1;
            }
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Up)
        {
            m_Selected_Skill = std::max((int)(m_Selected_Skill - m_Size.x), 0);
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            m_Selected_Skill = std::min((int)(m_Selected_Skill + m_Size.x), (int)Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills.size() - 1);
            Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
        }
    }
}

void GameState_Skill::mt_Update(float delta_time_s)
{
    GameStateDisplayer::mt_Update(delta_time_s);
}

void GameState_Skill::mt_Draw(sf::RenderTarget& target)
{
    sf::Vector2f l_Pos;
    sf::Text l_Text;
    sf::CircleShape l_Cursor;
    int l_Cnt(0);
    sf::FloatRect l_Bounds;
    sf::RectangleShape l_Rect;
    auto l_fn_Draw_Title = [&](const sf::FloatRect& bounds)
    {
        sf::Vector2f l_Offset;

        l_Rect.setFillColor(m_Color_Theme[1]);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        l_Text.setCharacterSize(40);
        l_Text.setString("Compétences");
        l_Text.setOrigin(l_Text.getGlobalBounds().width / 2.0f, 0.0f);
        l_Text.setPosition(bounds.left + bounds.width / 2.0f, bounds.top + m_Margin);
        target.draw(l_Text);
    };
    auto l_fn_Draw_Skill = [&](const sf::FloatRect& bounds)
    {
        sf::Vector2f l_Offset;

        l_Text.setCharacterSize(25);
        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);


        l_Offset.x = bounds.width / m_Size.x - m_Margin * 2.0f;
        l_Offset.y = bounds.height / m_Size.y - m_Margin * 2.0f;

        l_Pos.x = bounds.left + m_Margin;
        l_Pos.y = bounds.top + m_Margin;
        l_Text.setFont(*m_Font);
        l_Text.setFillColor(sf::Color::White);

        l_Rect.setSize({l_Offset.x, l_Offset.y});

        for (auto& q : Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills)
        {
            ISkill* l_Skill = q.m_Resource;
            l_Rect.setPosition(l_Pos);

            l_Text.setString(l_Skill->m_Name);
            l_Text.setOrigin(l_Text.getGlobalBounds().width / 2.0f, 0.0f);
            l_Text.setPosition(l_Rect.getGlobalBounds().left + l_Rect.getGlobalBounds().width / 2.0f , l_Pos.y);

            if (m_Selected_Skill == l_Cnt)
            {
                l_Rect.setFillColor(m_Selected_Color);
            }
            else
            {
                l_Rect.setFillColor(m_Inactive_Rect_Filling_Color);
            }

            target.draw(l_Rect);
            target.draw(l_Text);

            l_Pos.x += l_Offset.x + m_Margin * 2.0f;
            if (l_Pos.x >= bounds.left + bounds.width)
            {
                l_Pos.x = bounds.left + m_Margin;
                l_Pos.y += l_Offset.y + m_Margin * 2.0f;
            }

            l_Cnt++;
        }

        l_Text.setFillColor(sf::Color::White);
        l_Text.setOrigin(0.0f, 0.0f);



    };
    auto l_fn_Draw_Description = [&](const sf::FloatRect& bounds)
    {
        l_Rect.setFillColor(sf::Color::Black);
        l_Rect.setOutlineColor(sf::Color::White);
        l_Rect.setOutlineThickness(3.0f);
        l_Rect.setPosition(bounds.left, bounds.top);
        l_Rect.setSize({bounds.width, bounds.height});
        target.draw(l_Rect);

        if ((m_Selected_Skill >= 0) && (m_Selected_Skill < Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills.size()))
        {
            ISkill* l_Skill = Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills[m_Selected_Skill].m_Resource;

            l_Pos.x = l_Rect.getGlobalBounds().left + m_Margin;
            l_Pos.y = l_Rect.getGlobalBounds().top + m_Margin;
            l_Text.setCharacterSize(15);

            l_Text.setPosition(l_Pos);
            l_Text.setString("Description:");
            target.draw(l_Text);
            l_Pos.y += l_Text.getGlobalBounds().height;
            l_Text.setString(l_Skill->m_Description);
            l_Text.setPosition(l_Pos);
            target.draw(l_Text);
            l_Pos.y += l_Text.getGlobalBounds().height + m_Margin;

            l_Text.setString("Psy: " + std::to_string(l_Skill->m_Cost));
            l_Text.setPosition(l_Pos);
            target.draw(l_Text);
            l_Pos.y += l_Text.getGlobalBounds().height + m_Margin;

            l_Text.setString("Puissance: " + std::to_string(l_Skill->m_Damage));
            l_Text.setPosition(l_Pos);
            target.draw(l_Text);
            l_Pos.y += l_Text.getGlobalBounds().height + m_Margin;

            l_Text.setString("Distance: " + fn_To_String(l_Skill->m_Distance_Max));
            l_Text.setPosition(l_Pos);
            target.draw(l_Text);
            l_Pos.y += l_Text.getGlobalBounds().height + m_Margin;

            l_Text.setString("Cibles: " + std::to_string(l_Skill->m_Tgt_Max_Count));
            l_Text.setPosition(l_Pos);
            target.draw(l_Text);
            l_Pos.y += l_Text.getGlobalBounds().height + m_Margin;
        }
    };

    l_Cursor.setFillColor(sf::Color::White);
    l_Cursor.setPointCount(3);
    l_Cursor.setRadius(5.0f);


    target.setView(target.getDefaultView());

    l_Pos.x = m_Margin;
    l_Pos.y = m_Margin;
    l_Text.setFont(*m_Font);
    l_Text.setCharacterSize(30);
    l_Pos.x = m_Margin;
    l_Pos.y = 60.0f;

    l_Bounds.left = m_Margin;
    l_Bounds.width = target.getSize().x - 2 * l_Bounds.left;
    l_Bounds.top = m_Margin;
    l_Bounds.height = 75.0f - m_Margin;

    l_fn_Draw_Title(l_Bounds);

    l_Bounds.top += l_Bounds.height + m_Margin;
    l_Bounds.height = 250.0f - m_Margin;
    l_fn_Draw_Skill(l_Bounds);

    //l_Bounds.top = target.getSize().y - 250.0f;
    l_Bounds.top += l_Bounds.height + m_Margin;
    l_Bounds.height = target.getSize().y - l_Bounds.top - m_Margin;


    l_fn_Draw_Description(l_Bounds);
}

ISkill* GameState_Skill::mt_Get_Selected_Skill(void)
{
    ISkill* l_Skill(nullptr);

    if ((m_Selected_Skill >= 0) && (m_Selected_Skill < Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills.size()))
    {
        l_Skill = Context::smt_Get().m_Engine->m_Player->m_Gameplay_Data.m_Skills[m_Selected_Skill].m_Resource;
    }

    return l_Skill;
}






void GameState_EndGame::mt_Create(void)
{
    //
}

void GameState_EndGame::mt_Destroy(void)
{
    //
}

void GameState_EndGame::mt_Handle_Event(sf::Event& event)
{
    if (Context::smt_Get().m_Dialog->mt_Has_Event_Priority() == true)
    {
        Context::smt_Get().m_Dialog->mt_Handle_Event(event);
    }
    else if (Context::smt_Get().m_Engine->m_Script.m_User_Ctrl == true)
    {
        Context::smt_Get().m_Engine->m_State = GameStateType::Close;
    }
    else if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Space))
    {
        Context::smt_Get().m_Engine->m_State = GameStateType::Close;
    }
}

void GameState_EndGame::mt_Update(float delta_time_s)
{
    Context::smt_Get().m_Engine->m_Script.mt_Process_Command(delta_time_s);
    Context::smt_Get().m_Dialog->mt_Update(delta_time_s);

    if (Context::smt_Get().m_Engine->m_Script.m_User_Ctrl == true)
    {
        Context::smt_Get().m_Engine->m_State = GameStateType::Close;
    }
}

void GameState_EndGame::mt_Draw(sf::RenderTarget& target)
{
    target.setView(target.getDefaultView());
    Context::smt_Get().m_Dialog->mt_Draw(target);
}


