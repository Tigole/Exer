#ifndef _INTERPOLATION_HPP
#define _INTERPOLATION_HPP 1

#include <algorithm>
#include <iostream>
#include <typeinfo>

#include <SFML/Graphics.hpp>

class IInterpolator;

/// http://www.iquilezles.org/www/articles/functions/functions.htm

class IInterpolator
{
public:
    template<typename T>
    T mt_Interpolate(float current_time_s, float total_time_s, const T& start, const T& final)
    {
        T l_Ret;
        float l_Progression;

        l_Progression = std::min(1.0f, std::max(0.0f, mt_Progression(std::min(1.0f, std::max(0.0f, current_time_s / total_time_s)))));

        l_Ret = l_Progression * final + (1.0f - l_Progression) * start;

        return l_Ret;
    }
protected:
    virtual float mt_Progression(float x) = 0;
};

sf::Vector2f fn_Interpolate(IInterpolator* interpolator, float current_time_s, float total_tims_s, const sf::Vector2f& start, const sf::Vector2f& final);
sf::Color fn_Interpolate(IInterpolator* interpolator, float current_time_s, float total_tims_s, const sf::Color& start, const sf::Color& final);
template<typename T>
T fn_Interpolate(IInterpolator* interpolator, float current_time_s, float total_tims_s, const T& start, const T& final)
{
    return interpolator->mt_Interpolate(current_time_s, total_tims_s, start, final);
}









class Interpolator_Linear : public IInterpolator
{
protected:
    float mt_Progression(float x) override;
};

class Interpolator_Polynomial : public IInterpolator
{
public:
    Interpolator_Polynomial(const std::vector<float>& poly);

protected:
    float mt_Progression(float x) override;

    std::vector<float> m_Poly;
};

class Interpolator_Power : public IInterpolator
{
public:
    Interpolator_Power(float pow);
protected:
    float mt_Progression(float x) override;

    float m_Pow;
};

class Interpolator_Cos : public IInterpolator
{
protected:
    float mt_Progression(float x) override;
};

class Interpolator_Parabola : public IInterpolator
{
public:
    Interpolator_Parabola(float power);
protected:
    float mt_Progression(float x);

    float m_Power;
};

class Interpolator_Gain : public IInterpolator
{
public:
    Interpolator_Gain(float k);
protected:
    float mt_Progression(float x);
private:
    float m_Gain;
};


#endif // _INTERPOLATION_HPP
