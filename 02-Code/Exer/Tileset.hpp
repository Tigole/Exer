#ifndef _TILESET_HPP
#define _TILESET_HPP 1

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include "Managers/ResourceManager.hpp"

class TilesetManager;

class Tileset
{
    friend TilesetManager;

public:
    Tileset() : m_Solid(nullptr), m_Texture(), m_Column_Count(0), m_Raw_Count(0)
    {}

    ~Tileset()
    {
        mt_Clear();
    }

    void mt_Clear()
    {
        if (m_Solid != nullptr)
        {
            delete[] m_Solid;
        }
    }

    sf::IntRect mt_Get_Rect(int tile_x, int tile_y);
    bool mt_Is_Solid(int tile_x, int tile_y)
    {
        if ((tile_x >= 0) && (tile_x < m_Column_Count) && (tile_y >= 0) && (tile_y < m_Raw_Count))
        {
            return m_Solid[tile_y * m_Column_Count + tile_x];
        }
        return false;
    }
    bool mt_Is_Solid(int tile_index)
    {
        if ((tile_index >= 0) && (tile_index < (m_Column_Count * m_Raw_Count)))
        {
            return m_Solid[tile_index];
        }
        return false;
    }

    sf::Vector2f mt_Cell_To_Pixel(sf::Vector2f cell = sf::Vector2f(1.0f, 1.0f))
    {
        cell.x *= m_Tile_Size.x;
        cell.y *= m_Tile_Size.y;
        return cell;
    }

    sf::Vector2f mt_Pixel_To_Cell(sf::Vector2f pxl)
    {
        pxl.x /= m_Tile_Size.x;
        pxl.y /= m_Tile_Size.y;

        return pxl;
    }

    void mt_Cell_To_Pixel(float& x, float& y)
    {
        sf::Vector2f l_v(x, y);

        l_v = mt_Cell_To_Pixel(l_v);

        x = l_v.x;
        y = l_v.y;
    }

    void mt_Pixel_To_Cell(float& x, float& y)
    {
        sf::Vector2f l_v(x, y);

        l_v = mt_Pixel_To_Cell(l_v);

        x = l_v.x;
        y = l_v.y;
    }

    int m_Column_Count;
    int m_Raw_Count;
private:
    sf::Vector2i m_Tile_Size;
public:
    Resource<sf::Texture> m_Texture;
    bool* m_Solid;
};

#endif // _TILESET_HPP
