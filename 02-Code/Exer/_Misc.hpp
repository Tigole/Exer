#ifndef _MISC_HPP
#define _MISC_HPP 1

#include <vector>
#include <string>

#include <istream>
#include <ostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>

#include "FilePacker/FilePacker.hpp"

class TiXmlDocument;

std::vector<std::pair<std::string, std::string>> fn_Get_Files(const std::string& path);
bool fn_Get_Line(std::stringstream& ss, std::string& line);
bool fn_Parse_XML_Document(TiXmlDocument& doc, const jaja::fp::File& f);
float fn_Distance(const sf::Vector2f& a, const sf::Vector2f& b);
//sf::Vector2f fn_Get_Direction()



std::ostream& operator<<(std::ostream& o, const sf::Color& c);
std::istream& operator>>(std::istream& i, sf::Color& c);

template<typename T>
std::ostream& operator<<(std::ostream& o, const sf::Vector2<T>& v)
{
    o << v.x << ' ' << v.y;
    return o;
}

template<typename T>
std::istream& operator>>(std::istream& i, sf::Vector2<T>& v)
{
    i >> v.x >> v.y;
    return i;
}


sf::String fn_Color_To_RichText(const sf::Color& c);
sf::String fn_Color_To_RichText(int r, int g, int b);
class Creature;
sf::String fn_Dialog(const Creature* speaker, const sf::Color& speaker_color, const sf::String& text);
sf::String fn_Dialog(const Creature* speaker, const sf::String& text);
std::vector<sf::String> fn_Dialog(const Creature* speaker, const std::vector<sf::String>& text);
sf::String fn_City_Name(const sf::String& city_name, const sf::Color& previous_color = sf::Color::White);
sf::String fn_Command_Key(const sf::String& key, const sf::Color& previous_color = sf::Color::White);
sf::String fn_Quest_Object(const sf::String& obj, const sf::Color& previous_color = sf::Color::White);




/// http://www.iquilezles.org/www/articles/functions/functions.htm
float fn_Cubic_Pulse(float center, float width, float x);

#endif // _MISC_HPP
