#include "Interpolation.hpp"


/**

pow: overshoot  | undershoot| ~= cos
x6 :            |           | 0,000000135
x5 :            |           | 9,4903926086
x4 :            |           | -23,7259815712
x3 : 12,8       | 5,3333    | 17,5602338845
x2 : -21,829    | -4,1143   | -2,6143692395
x1 : 10,029     | -0,219    | 0,289724306
x0 : 0          | 0         | 0

lent -> rapide -> lent
0.5 * (1 - cos(3.1416*x)

**/



sf::Vector2f fn_Interpolate(IInterpolator* interpolator, float current_time_s, float total_tims_s, const sf::Vector2f& start, const sf::Vector2f& final)
{
    sf::Vector2f l_Ret;

    l_Ret.x = interpolator->mt_Interpolate(current_time_s, total_tims_s, start.x, final.x);
    l_Ret.y = interpolator->mt_Interpolate(current_time_s, total_tims_s, start.y, final.y);

    return l_Ret;
}

sf::Color fn_Interpolate(IInterpolator* interpolator, float current_time_s, float total_tims_s, const sf::Color& start, const sf::Color& final)
{
    sf::Color l_Ret;

    l_Ret.r = interpolator->mt_Interpolate(current_time_s, total_tims_s, start.r, final.r);
    l_Ret.g = interpolator->mt_Interpolate(current_time_s, total_tims_s, start.g, final.g);
    l_Ret.b = interpolator->mt_Interpolate(current_time_s, total_tims_s, start.b, final.b);
    l_Ret.a = interpolator->mt_Interpolate(current_time_s, total_tims_s, start.a, final.a);

    return l_Ret;
}



float Interpolator_Linear::mt_Progression(float x)
{
    return x;
}

Interpolator_Polynomial::Interpolator_Polynomial(const std::vector<float>& poly)
 :  m_Poly(poly)
{}

float Interpolator_Polynomial::mt_Progression(float x)
{
    float l_Ret(0.0f);

    for (std::size_t ii = 0; ii < m_Poly.size(); ii++)
    {
        l_Ret += m_Poly[ii] * std::pow(x, ii);
    }

    return l_Ret;
}

Interpolator_Power::Interpolator_Power(float pow) : m_Pow(pow) {}

float Interpolator_Power::mt_Progression(float x)
{
    return std::pow(x, m_Pow);
}

float Interpolator_Cos::mt_Progression(float x)
{
    return 0.5f * (1 - cos(2.0 * 3.1416 * x));
}


Interpolator_Parabola::Interpolator_Parabola(float power) : m_Power(power) {}

float Interpolator_Parabola::mt_Progression(float x)
{
    return pow(4.0f * x * (1.0f - x), m_Power);
}



Interpolator_Gain::Interpolator_Gain(float k) : m_Gain(k) {}

float Interpolator_Gain::mt_Progression(float x)
{
    const float a = 0.5f * pow(2.0f * ((x < 0.5f) ? x : 1.0f - x), m_Gain);
    return (x < 0.5f) ? a : 1.0f - a;
}

