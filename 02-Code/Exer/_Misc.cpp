#include "_Misc.hpp"
#include "Dynamics.hpp"

#include <unistd.h>
#include <dir.h>
#include <dirent.h>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "tinyxml.h"

std::vector<std::pair<std::string, std::string>> fn_Get_Files(const std::string& path)
{
    std::vector<std::pair<std::string, std::string>> l_Ret;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            std::string l_str(ent->d_name);
            if ((l_str != ".") && (l_str != ".."))
            {
                l_Ret.push_back(std::make_pair(l_str.substr(0, l_str.find_last_of('.')), l_str.substr(l_str.find_last_of('.') + 1)));
            }
        }
        closedir (dir);
    }

    return l_Ret;
}

bool fn_Get_Line(std::stringstream& ss, std::string& line)
{
    bool l_b_Ret;

    l_b_Ret = !std::getline(ss, line, '\n').fail();

    while ((line.back() == '\r') || (line.back() == '\n'))
    {
        line.pop_back();
    }

    return l_b_Ret;
}

bool fn_Parse_XML_Document(TiXmlDocument& doc, const jaja::fp::File& f)
{
    std::string l_String(f.mt_To_String());

    return doc.Parse(l_String.c_str());
}

std::ostream& operator<<(std::ostream& o, const sf::Color& c)
{
    return o;
}

std::istream& operator>>(std::istream& i, sf::Color& c)
{
    int l_tmp;
    i >> l_tmp;
    c.r = l_tmp;
    i >> l_tmp;
    c.g = l_tmp;
    i >> l_tmp;
    c.b = l_tmp;
    return i;
}


sf::String fn_Color_To_RichText(const sf::Color& c)
{
    sf::String l_Ret;
    std::stringstream l_ss;

    l_ss << std::hex << std::setw(2) << std::setfill('0') << (int)c.r << std::setw(2) << std::setfill('0') << (int)c.g << std::setw(2) << std::setfill('0') << (int)c.b;
    l_Ret = "#" + l_ss.str() + ' ';

    //std::cout << "fn_Color_To_RichText: " << l_Ret.toAnsiString() << '\n';

    return l_Ret;
}

sf::String fn_Color_To_RichText(int r, int g, int b)
{
    return fn_Color_To_RichText(sf::Color(r, g, b));
}

sf::String fn_Dialog(const Creature* speaker, const sf::Color& speaker_color, const sf::String& text)
{
    sf::String l_Ret;

    l_Ret = fn_Color_To_RichText(speaker_color) + "_" + ((speaker != nullptr) ? speaker->m_Name : "???") + "_" + fn_Color_To_RichText(sf::Color::White) + " :\n" + text;

    //std::cout << "fn_Dialog: " << l_Ret.toAnsiString() << '\n';

    return l_Ret;
}

sf::String fn_Dialog(const Creature* speaker, const sf::String& text)
{
    return fn_Dialog(speaker, (speaker != nullptr) ? speaker->m_Dialog_Color : sf::Color::White, text);
}

sf::String fn_City_Name(const sf::String& city_name, const sf::Color& previous_color)
{
    return fn_Color_To_RichText(sf::Color(0xFC, 0x5D, 0x5D)) + "*" + city_name + "*" + fn_Color_To_RichText(previous_color);
}

sf::String fn_Command_Key(const sf::String& key, const sf::Color& previous_color)
{
    return fn_Color_To_RichText(sf::Color(0xF4, 0x66, 0x1B)) + "*" + key + "*" + fn_Color_To_RichText(previous_color);
}

sf::String fn_Quest_Object(const sf::String& obj, const sf::Color& previous_color)
{
    return fn_Color_To_RichText(sf::Color(50, 205, 50)) + "*" + obj + "*" + fn_Color_To_RichText(previous_color);
}
