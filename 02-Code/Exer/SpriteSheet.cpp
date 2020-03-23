#include "SpriteSheet.hpp"
#include "Context.hpp"
#include <fstream>
#include <sstream>

SpriteSheet::SpriteSheet(){}

bool SpriteSheet::mt_Load_From_File(const std::string& ref_file)
{
    std::stringstream l_file;
    jaja::fp::File l_File_Buffer;

    Context::smt_Get().m_Packer.mt_Get_File(ref_file, l_File_Buffer);

    l_file.str(std::string(l_File_Buffer.m_Buffer, l_File_Buffer.m_Size));

    l_file >> m_Texture_Id >> m_XX >> m_YY;

    m_Texture = Context::smt_Get().m_Textures.mt_Get_Resource(m_Texture_Id);

    return true;
}

sf::IntRect SpriteSheet::mt_Get_Rect(std::size_t xx, std::size_t yy)
{
    sf::IntRect l_Ret;
    int l_dx = m_Texture->getSize().x / m_XX;
    int l_dy = m_Texture->getSize().y / m_YY;

    l_Ret.top = yy * l_dy;
    l_Ret.left = xx * l_dx;
    l_Ret.width = l_dx;
    l_Ret.height = l_dy;

    return l_Ret;
}

sf::Texture& SpriteSheet::mt_Get_Texture(void)
{
    return *m_Texture;
}



bool SpriteSheetAnimator::mt_Load_From_File(const std::string& ref_file)
{
    std::stringstream l_file;
    std::string l_sprite_sheet;
    jaja::fp::File l_File_Buffer;

    Context::smt_Get().m_Packer.mt_Get_File(ref_file, l_File_Buffer);

    l_file.str(std::string(l_File_Buffer.m_Buffer, l_File_Buffer.m_Size));

    l_file >> l_sprite_sheet >> m_Offset.x >> m_Offset.y >> m_Switch_Time_Ref_s;

    m_Sheet = Context::smt_Get().m_SpriteSheets.mt_Get_Resource(l_sprite_sheet);

    return true;
}

sf::IntRect SpriteSheetAnimator::mt_Get_Rect(float elapsed_time_s, const sf::Vector2f& orientation, bool moving, SpriteSheetAnimatorData& data)
{
    std::size_t xx[4] = {1, 2, 1, 0};

    data.m_Switch_Time_s -= elapsed_time_s;
    if (moving == true)
    {
        if (data.m_Switch_Time_s < 0.0f)
        {
            data.m_Switch_Time_s = m_Switch_Time_Ref_s;
            data.m_Old_Offset.x++;
            if (data.m_Old_Offset.x >= 4)
                data.m_Old_Offset.x = 0;
        }
    }
    else
    {
        data.m_Old_Offset.x = 0;
        data.m_Switch_Time_s = m_Switch_Time_Ref_s;
    }

    if (fabs(orientation.x) > fabs(orientation.y))
    {
        if (orientation.x > 0.0f)
        {
            data.m_Old_Offset.y = static_cast<int>(SpriteSheet_Direction::East);
            data.m_Facing_Dir = SpriteSheet_Direction::East;
        }
        if (orientation.x < 0.0f)
        {
            data.m_Old_Offset.y = static_cast<int>(SpriteSheet_Direction::West);
            data.m_Facing_Dir = SpriteSheet_Direction::West;
        }
    }
    else if (orientation.y > 0.0f)
    {
        data.m_Old_Offset.y = static_cast<int>(SpriteSheet_Direction::South);
        data.m_Facing_Dir = SpriteSheet_Direction::South;
    }
    else if (orientation.y < 0.0f)
    {
        data.m_Old_Offset.y = static_cast<int>(SpriteSheet_Direction::North);
        data.m_Facing_Dir = SpriteSheet_Direction::North;
    }

    data.m_Old_Offset.y = static_cast<int>(data.m_Facing_Dir);

    return m_Sheet->mt_Get_Rect(xx[data.m_Old_Offset.x] + m_Offset.x, data.m_Old_Offset.y + m_Offset.y);
}

sf::FloatRect SpriteSheetAnimator::mt_Get_HitBox_Margins(const SpriteSheetAnimatorData& data)
{
    sf::FloatRect l_Ret;

    l_Ret.top = 0.0f;
    l_Ret.left = 0.0f;
    l_Ret.width = 0.0f;
    l_Ret.height = 0.0f;

    return l_Ret;
}
