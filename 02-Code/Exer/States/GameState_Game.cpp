#include "GameState_Game.hpp"
#include "../GameEngine.hpp"

/*void GameState_Game::mt_On_Entry()
{
    //
}*/

void GameState_Game::mt_Create(void)
{
    m_Dialog.m_Dialog_Font = Context::smt_Get().m_Fonts.mt_Get_Resource("BOOKOS");
    m_Dialog.m_Dialog_Pos = Context::smt_Get().m_Engine->m_Camera.mt_Get_View().getCenter();
    m_Dialog.m_Show_Dialog = false;
    m_Dialog.m_Show_Choices = false;
    m_Dialog.m_Current_Line = 0;
    m_Dialog.m_Dialog.resize(0);

    Context::smt_Get().m_Dialog = &m_Dialog;

    m_Shader = Context::smt_Get().m_Shaders.mt_Get_Resource("Shader");
}

void GameState_Game::mt_Destroy(void)
{
    Context::smt_Get().m_Shaders.mt_Release(m_Shader);
    Context::smt_Get().m_Fonts.mt_Release(m_Dialog.m_Dialog_Font);
}

void GameState_Game::mt_Handle_Event(sf::Event& event)
{
    if (m_Dialog.mt_Has_Event_Priority())
    {
        m_Dialog.mt_Handle_Event(event);
    }
    else
    {
        Creature* l_Player = Context::smt_Get().m_Engine->m_Player;
        if (event.type == sf::Event::KeyPressed)
        {
            if (Context::smt_Get().m_Engine->m_Script.m_User_Ctrl)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    l_Player->m_Desired_Vel.y =  -l_Player->m_Speed;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    l_Player->m_Desired_Vel.y = l_Player->m_Speed;
                }
                else if (event.key.code == sf::Keyboard::Left)
                {
                    l_Player->m_Desired_Vel.x = -l_Player->m_Speed;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    l_Player->m_Desired_Vel.x = l_Player->m_Speed;
                }
            }
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
            {
                l_Player->m_Desired_Vel.y = 0.0f;
            }
            else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
            {
                l_Player->m_Desired_Vel.x = 0.0f;
            }
            else if (event.key.code == sf::Keyboard::Space)
            {
                SpriteSheet_Direction l_Facing_Dir = l_Player->mt_Get_Facing_Dir();
                sf::Vector2f l_Test_Pos(l_Player->m_Pos);

                switch(l_Facing_Dir)
                {
                case SpriteSheet_Direction::South:
                    l_Test_Pos.x += 0.5f;
                    l_Test_Pos.y += 1.5f;
                    break;
                case SpriteSheet_Direction::West:
                    l_Test_Pos.x -= 0.5f;
                    l_Test_Pos.y += 0.5f;
                    break;
                case SpriteSheet_Direction::East:
                    l_Test_Pos.x += 1.5f;
                    l_Test_Pos.y += 0.5f;
                    break;
                case SpriteSheet_Direction::North:
                    l_Test_Pos.x += 0.5f;
                    l_Test_Pos.y -= 0.5f;
                    break;
                }

                for (auto& d : Context::smt_Get().m_Engine->m_Dyn)
                {
                    Dynamic* l_d = d.m_Resource;
                    sf::FloatRect l_hit_box = l_d->mt_Get_Hit_Box();
                    if (l_hit_box.contains(l_Test_Pos))
                    {
                        if ((Context::smt_Get().m_Engine->m_Script.m_User_Ctrl == true) && (l_d->m_Friendly == true))
                        {
                            bool l_test(false);
                            for (auto& q : Context::smt_Get().m_Engine->m_System_Quest.m_Quests)
                            {
                                Quest* l_q = q.m_Resource;

                                if (l_q->mt_On_Interaction(Context::smt_Get().m_Engine->m_Dyn, l_d, Quest_Nature::Talking))
                                {
                                    l_test = true;
                                    break;
                                }
                            }
                            if (!l_test)
                                l_test = Context::smt_Get().m_Engine->m_Map->mt_On_Interaction(Context::smt_Get().m_Engine->m_Dyn, l_d, InteractionNature::Talking);
                            if (!l_test)
                                l_d->mt_OnInteract(l_Player);
                        }
                        else
                        {
                            /// Attack ?
                        }
                    }
                }
            }
            else if (event.key.code == sf::Keyboard::I)
            {
                Context::smt_Get().m_Engine->m_State = GameStateType::Display_Inventory;
                Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
            }
            else if (event.key.code == sf::Keyboard::O)
            {
                Context::smt_Get().m_Engine->m_State = GameStateType::Display_Quests;
                Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
            }
            else if (event.key.code == sf::Keyboard::P)
            {
                Context::smt_Get().m_Engine->m_Skill_State.m_Previous_State = GameStateType::Game;
                Context::smt_Get().m_Engine->m_State = GameStateType::Display_Skills;
                Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Validate_String, {0.0f, 0.0f, 0.0f}, true);
            }
        }
    }
}

void GameState_Game::mt_Update(float delta_time_s)
{
    std::function<void(sf::Vector2f& new_pos, Dynamic* obj)> l_fn_Handle_Map_Collision = [&](sf::Vector2f& new_pos, Dynamic* obj)
    {
        if (obj->m_Solid_Map == true)
        {
            float l_Border = 0.1f;
            sf::Vector2f l_Old_Vel = obj->m_Vel;
            if (obj->m_Vel.x <= 0.0f)
            {
                if (    Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x, obj->m_Pos.y + l_Border)
                    ||  Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x, obj->m_Pos.y + (1.0f - l_Border)))
                {
                    new_pos.x = (int)new_pos.x + 1;
                    obj->m_Vel.x = 0.0f;
                }
            }
            else
            {
                if (    Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x + (1.0f), obj->m_Pos.y + l_Border)
                    ||  Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x + (1.0f), obj->m_Pos.y + (1.0f - l_Border)))
                {
                    new_pos.x = (int)new_pos.x;
                    obj->m_Vel.x = 0.0f;
                }
            }

            if (obj->m_Vel.y <= 0.0f)
            {
                if (    Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x + l_Border, new_pos.y)
                    ||  Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x + (1.0f - l_Border), new_pos.y))
                {
                    new_pos.y = (int)new_pos.y + 1;
                    obj->m_Vel.y = 0.0f;
                }
            }
            else
            {
                if (    Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x + l_Border, new_pos.y + 1.0f)
                    ||  Context::smt_Get().m_Engine->m_Map->mt_Is_Solid(new_pos.x + (1.0f - l_Border), new_pos.y + 1.0f))
                {
                    new_pos.y = (int)new_pos.y;
                    obj->m_Vel.y = 0.0f;
                }
            }

            if ((obj == Context::smt_Get().m_Engine->m_Player) && (l_Old_Vel != obj->m_Vel))
            {
                //Context::smt_Get().m_System_Sound.mt_Play_Sound(SystemSound::m_Buzz_String, sf::Vector3f(0.0f, 0.0f, 0.0f), true);
            }
        }
    };
    std::function<void(sf::Vector2f& new_pos, Dynamic* obj)> l_fn_Handle_Dyn_Collision = [&](sf::Vector2f& new_pos, Dynamic* obj)
    {
        for (auto& d : Context::smt_Get().m_Engine->m_Dyn)
        {
            Dynamic* l_Dyn = d.m_Resource;
            if (l_Dyn != obj)
            {
                if (l_Dyn->m_Solid_Dyn && obj->m_Solid_Dyn)
                {
                    if (    ((new_pos.x < (l_Dyn->m_Pos.x + 1.0f)) && ((new_pos.x + 1.0f) > (l_Dyn->m_Pos.x)))
                        &&  ((obj->m_Pos.y < (l_Dyn->m_Pos.y + 1.0f)) && ((obj->m_Pos.y + 1.0f) > (l_Dyn->m_Pos.y))))
                    {
                        if (obj->m_Vel.x < 0.0f)
                            new_pos.x = l_Dyn->m_Pos.x + 1.0f;
                        else
                            new_pos.x = l_Dyn->m_Pos.x - 1.0f;
                        obj->m_Vel.x = 0.0f;
                    }

                    if (    ((new_pos.x < (l_Dyn->m_Pos.x + 1.0f)) && ((new_pos.x + 1.0f) > (l_Dyn->m_Pos.x)))
                        &&  ((new_pos.y < (l_Dyn->m_Pos.y + 1.0f)) && ((new_pos.y + 1.0f) > (l_Dyn->m_Pos.y))))
                    {
                        if (obj->m_Vel.y < 0.0f)
                            new_pos.y = l_Dyn->m_Pos.y + 1.0f;
                        else
                            new_pos.y = l_Dyn->m_Pos.y - 1.0f;
                        obj->m_Vel.y = 0.0f;
                    }
                }
                else
                {
                    if (l_Dyn == Context::smt_Get().m_Engine->m_Dyn[0].m_Resource)
                    {
                        sf::FloatRect l_Rect1(obj->mt_Get_Hit_Box());
                        sf::FloatRect l_Rect2(l_Dyn->mt_Get_Hit_Box());

                        if ((Context::smt_Get().m_Engine->m_Script.m_Check_Dyn_Collision == true) && (obj->mt_Get_Hit_Box().contains(l_Dyn->m_Pos)))
                        {
                            bool l_Processed(false);

                            /// Interaction with quest ?
                            for (auto& q : Context::smt_Get().m_Engine->m_System_Quest.m_Quests)
                            {
                                if ((q->m_Completed == false) && (q->mt_On_Interaction(Context::smt_Get().m_Engine->m_Dyn, obj, Quest_Nature::Walking)))
                                {
                                    l_Processed = true;
                                    break;
                                }
                            }

                            /// Interaction with map
                            if (!l_Processed)
                            {
                                l_Processed = Context::smt_Get().m_Engine->m_Map->mt_On_Interaction(Context::smt_Get().m_Engine->m_Dyn, obj, InteractionNature::Walking);
                            }

                            if (!l_Processed)
                            {
                                obj->mt_OnInteract(Context::smt_Get().m_Engine->m_Player);
                            }
                        }
                    }
                }
            }
        }
    };

    Context::smt_Get().m_Engine->m_Animation.mt_Update(delta_time_s);

    for (auto& o : Context::smt_Get().m_Engine->m_Dyn)
    {
        Dynamic* l_Obj = o.m_Resource;

        l_Obj->m_Vel = l_Obj->m_Desired_Vel;
        if (l_Obj->m_Vel != sf::Vector2f(0.0f, 0.0f))
        {
            l_Obj->m_Vel /= (float)sqrt((l_Obj->m_Vel.x * l_Obj->m_Vel.x) + (l_Obj->m_Vel.y * l_Obj->m_Vel.y));
            l_Obj->m_Vel *= l_Obj->m_Speed;
        }
        sf::Vector2f l_New_Pos = l_Obj->m_Pos + l_Obj->m_Vel * delta_time_s;

        /// Map Collision
        l_fn_Handle_Map_Collision(l_New_Pos, l_Obj);

        /// Dyn Collision
        l_fn_Handle_Dyn_Collision(l_New_Pos, l_Obj);

        l_Obj->m_Pos = l_New_Pos;

        l_Obj->mt_Update(delta_time_s);
    }

    /*auto i = std::remove_if(Context::smt_Get().m_Engine->m_System_Quest.m_Quests.begin(), Context::smt_Get().m_Engine->m_System_Quest.m_Quests.end(), [](const Resource<Quest>& q){return q->m_Completed == true;});
    if (i != Context::smt_Get().m_Engine->m_System_Quest.m_Quests.end())
    {
        for (auto ii = i; ii != Context::smt_Get().m_Engine->m_System_Quest.m_Quests.end(); ii++)
            Context::smt_Get().m_Engine->m_System_Quest.m_Ended_Quests.push_back(ii->m_Resource->m_Quest_Id);
        Context::smt_Get().m_Engine->m_System_Quest.m_Quests.erase(i);
    }*/

    /*if (Context::smt_Get().m_Engine->m_Script.m_Camera_Ctrl)
    {
        Context::smt_Get().m_Engine->mt_Set_Camera_Center(Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(Context::smt_Get().m_Engine->m_Player->m_Pos + sf::Vector2f(0.5f, 0.5f)));
    }*/

    Context::smt_Get().m_Engine->m_Script.mt_Process_Command(delta_time_s);

    m_Dialog.mt_Update(delta_time_s);
    m_Dialog.m_Dialog_Pos = Context::smt_Get().m_Engine->m_Camera.mt_Get_View().getCenter();
    m_Dialog.m_Dialog_Pos.y = Context::smt_Get().m_Engine->m_Camera.mt_Get_View().getSize().x / 2;
}

void GameState_Game::mt_Draw(sf::RenderTarget& target)
{
    std::vector<sf::RectangleShape> l_Rect;
    std::vector<sf::Text> l_Text;
    const Creature* l_Player = Context::smt_Get().m_Engine->m_Player;
    sf::Vector2f l_Margin(20.0f, 10.0f);
    std::function<void(int, const sf::View&)> l_fn_Draw_Map = [&](int lvl, const sf::View& camera_view)
    {
        sf::Sprite l_Sprite;
        sf::IntRect l_Rect;
        Map* l_Map = Context::smt_Get().m_Engine->m_Map.m_Resource;
        Tileset* l_Tileset = l_Map->m_Tileset.m_Resource;
        sf::FloatRect l_View(camera_view.getCenter().x - camera_view.getSize().x / 2,
                             camera_view.getCenter().y - camera_view.getSize().y / 2,
                             camera_view.getSize().x,
                             camera_view.getSize().y);

        l_Rect.width = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(sf::Vector2f(1.0f, 1.0f)).x;
        l_Rect.height = Context::smt_Get().m_Engine->m_Map->m_Tileset->mt_Cell_To_Pixel(sf::Vector2f(1.0f, 1.0f)).y;
        l_Sprite.setTexture(*Context::smt_Get().m_Engine->m_Map->m_Tileset->m_Texture);

        l_Tileset->mt_Pixel_To_Cell(l_View.left, l_View.top);
        l_Tileset->mt_Pixel_To_Cell(l_View.width, l_View.height);

        for (int xx = std::max(0, (int)l_View.left); xx < std::min(l_Map->m_Width, (int)(l_View.left + l_View.width + 1)); xx++)
        {
            for (int yy = std::max(0, (int)l_View.top); yy < std::min(l_Map->m_Height, (int)(l_View.top + l_View.height + 1)); yy++)
            {
                int l_index = l_Map->mt_Get_Index(xx, yy, lvl);
                if (l_index != -1)
                {
                    l_Rect.left = (l_index % Context::smt_Get().m_Engine->m_Map->m_Tileset->m_Column_Count) * l_Rect.width;
                    l_Rect.top = (l_index / Context::smt_Get().m_Engine->m_Map->m_Tileset->m_Column_Count) * l_Rect.height;

                    l_Sprite.setTextureRect(l_Rect);
                    l_Sprite.setPosition(xx * l_Rect.width, yy * l_Rect.height);

                    target.draw(l_Sprite, m_Shader.m_Resource);
                }
            }
        }
    };

    target.setView(Context::smt_Get().m_Engine->m_Camera.mt_Get_View());

    m_Shader->setUniform("red", Context::smt_Get().m_Engine->m_Sky_Color.r / 255.0f);
    m_Shader->setUniform("green", Context::smt_Get().m_Engine->m_Sky_Color.g / 255.0f);
    m_Shader->setUniform("blue", Context::smt_Get().m_Engine->m_Sky_Color.b / 255.0f);


    if (Context::smt_Get().m_Engine->m_Map->m_Tileset->m_Texture == nullptr)
    {
        std::cout << "No tileset\n";
        return;
    }

    l_fn_Draw_Map(0, target.getView());
    l_fn_Draw_Map(1, target.getView());
    for (auto& d : Context::smt_Get().m_Engine->m_Dyn)
    {
        d->mt_Draw(target, m_Shader.m_Resource);
    }
    l_fn_Draw_Map(2, target.getView());

    Context::smt_Get().m_Engine->m_Animation.mt_Draw(target);

    target.setView(target.getDefaultView());

    if (    (Context::smt_Get().m_Engine->m_Script.m_User_Ctrl)
        &&  (Context::smt_Get().m_Engine->m_State == GameStateType::Game))
    {
        std::vector<std::string> l_Menus;
        sf::Vector2f l_v(0.0f, 0.0f);

        l_Menus.push_back("I : Inventaire");
        l_Menus.push_back("O : Quêtes");
        l_Menus.push_back("P : Compétences");

        l_Rect.resize(l_Menus.size());
        l_Text.resize(l_Menus.size());
        for (std::size_t ii = 0; ii < l_Menus.size(); ii++)
        {
            l_Text[ii].setString(l_Menus[ii]);
            l_Text[ii].setFont(*m_Dialog.m_Dialog_Font);
            l_Text[ii].setOrigin(l_Text[ii].getGlobalBounds().width / 2.0f, 0.0f);

            l_v.x = std::max(l_v.x, l_Text[ii].getGlobalBounds().width);
            l_v.y = std::max(l_v.y, l_Text[ii].getGlobalBounds().height);
        }

        for (std::size_t ii = 0; ii < l_Text.size(); ii++)
        {
            l_Rect[ii].setFillColor(sf::Color(20, 20, 150));
            l_Rect[ii].setOutlineThickness(2.0f);
            l_Rect[ii].setOutlineColor(sf::Color::White);
            l_Rect[ii].setSize(l_v + l_Margin);
            l_Rect[ii].setPosition(l_Rect[ii].getGlobalBounds().width / 2 + (ii * l_Rect[ii].getGlobalBounds().width) + l_Rect[ii].getOutlineThickness(),
                                   l_Rect[ii].getGlobalBounds().height / 2 + l_Rect[ii].getOutlineThickness());
            l_Rect[ii].setOrigin(l_Rect[ii].getGlobalBounds().width / 2, l_Rect[ii].getGlobalBounds().height / 2);

            l_Text[ii].setPosition(l_Rect[ii].getPosition().x, l_Margin.y / 2.0f);
        }

        for (std::size_t ii = 0; ii < l_Text.size(); ii++)
        {
            target.draw(l_Rect[ii]);
            target.draw(l_Text[ii]);
        }
    }

    l_Text.resize(1);
    l_Rect.resize(1);
    l_Text[0].setCharacterSize(20);
    l_Text[0].setFont(*m_Dialog.m_Dialog_Font);
    l_Text[0].setOrigin(0.0f, 0.0f);
    l_Text[0].setString(l_Player->m_Name + "\n"
                        + "Vie: " + std::to_string(l_Player->m_Gameplay_Data.m_Health) + " / " + std::to_string(l_Player->m_Gameplay_Data.m_Health_Max) + "\n"
                        + "Psy: " + std::to_string(l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy) + " / " + std::to_string(l_Player->m_Gameplay_Data.m_Data_Aventure.m_Psy_Max));
    l_Text[0].setPosition(target.getSize().x - l_Text[0].getGlobalBounds().width - l_Margin.x / 2.0f, l_Margin.y / 2.0f);
    l_Rect[0].setSize({l_Text[0].getGlobalBounds().width + l_Margin.x, l_Text[0].getGlobalBounds().height + l_Margin.y});
    l_Rect[0].setOrigin(0.0f, 0.0f);
    l_Rect[0].setFillColor(sf::Color(20, 20, 150));
    l_Rect[0].setOutlineThickness(2.0f);
    l_Rect[0].setOutlineColor(sf::Color::White);
    l_Rect[0].setPosition(target.getSize().x - l_Rect[0].getGlobalBounds().width,
                          l_Rect[0].getOutlineThickness());

    target.draw(l_Rect[0]);
    target.draw(l_Text[0]);


    m_Dialog.mt_Draw(target);
}

void GameState_Game::mt_Draw_HUD(sf::RenderTarget& target) const
{
    //
}

void GameState_Game::mt_Draw_Gameplay_Data(sf::RenderTarget& target) const
{
    //
}
