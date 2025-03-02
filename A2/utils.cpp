#include "utils.hpp"

void normalize_velocity(float &vx, float &vy, float max_speed)
{
    float total_speed = (vx * vx + vy * vy);
    if (total_speed <= max_speed * max_speed)
        return;
    if (max_speed < 0)
        max_speed += -0.00001f;
    else
        max_speed -= 0.00001f;

    float alpha = std::atan2(vy, vx);
    // printf("vx: %f, vy: %f, alpha: %f\n", vx, vy, alpha);
    // printf("sin alpha: %f, cos alpha: %f\n", std::sin(alpha), std::cos(alpha));
    vx = max_speed * std::cos(alpha);
    vy = max_speed * std::sin(alpha);
    // printf("calculation %f", max_speed * std::sin(alpha));
    // printf("new vx: %f, new vy: %f\n", vx, vy);
}

void apply_friction(float &vx, float &vy, float df)
{
    vx *= df;
    vy *= df;
    if (std::abs(vx) <= 0.001f)
    {
        vx = 0.0f;
    }
    if (std::abs(vy) <= 0.001f)
    {
        vy = 0.0f;
    }
}

float clamp(float value, float min, float max)
{
    return std::max(min, std::min(value, max));
}

float distance(float x1, float y1, float x2, float y2)
{
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}