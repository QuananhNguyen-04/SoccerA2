// ball.cpp
#include "ball.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <SDL2/SDL.h>
#include <cmath>

Ball::Ball() {}

Ball::Ball(Texture *manager, SDL_Renderer *renderer) : renderer(renderer), radius(config::BALL_RADIUS),
                                                       start_pos({config::BORDER.x + (float)config::BORDER.w / 2,
                                                                  config::BORDER.y + (float)config::BORDER.h / 2}),
                                                       pos(start_pos),
                                                       vx(0),
                                                       vy(0),
                                                       d_factor(config::BALL_DAMPING_FACTOR),
                                                       ingoal{false, false, false},
                                                       manager(manager)
{
    // printf("ball pos, posx: %f, posy: %f\n", pos.x, pos.y);
    current_sprite = 0;
}

void drawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    std::vector<SDL_Point> points;
    for (int w = 0; w < int(radius * 2); w++)
    {
        for (int h = 0; h < int(radius * 2); h++)
        {
            int dx = int(radius - w); // horizontal offset
            int dy = int(radius - h); // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                points.push_back(SDL_Point{x + dx, y + dy});
                // SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
    SDL_RenderDrawPoints(renderer, points.data(), points.size());
}
void Ball::draw(bool change)
{

    int ball_sprite_index = static_cast<int>(TextureType::BALL_RIGHT1);

    if (change)
    {
        if (vx > 0.001 or vy > 0.001)
            current_sprite += 1;
        else if (vx < -0.001 or vy < -0.001)
        {
            current_sprite += 5;
        }
        current_sprite %= 4;
    }
    auto txtr = manager->get_texture(static_cast<TextureType>(ball_sprite_index + current_sprite));

    SDL_Rect dstrect = {static_cast<int>(pos.x - radius), static_cast<int>(pos.y - radius), static_cast<int>(2 * radius), static_cast<int>(2 * radius)};
    SDL_RenderCopy(renderer, txtr, NULL, &dstrect);
    // SDL_SetRenderDrawColor(renderer, config::WHITE.r, config::WHITE.g, config::WHITE.b, 255);
    // drawCircle(renderer, pos.x, pos.y, radius);
}

std::pair<SDL_Rect, float> Ball::collide(const std::vector<std::pair<SDL_Rect, float>> &rect_list)
{
    for (const auto &player : rect_list)
    {

        // Check for collision with each rectangle in rect_list
        // Implementation left out for brevity
        auto rect = player.first;
        float closest_x = clamp(pos.x, rect.x + float(rect.w) * 0.1, rect.x + rect.w * 0.9);
        float closest_y = clamp(pos.y, rect.y + float(rect.h) * 0.4, rect.y + rect.h);
        auto distance = std::pow(closest_x - pos.x, 2) + std::pow(closest_y - pos.y, 2);
        if (distance <= std::pow(radius, 2))
        {
            return player;
        }
    }
    return {SDL_Rect{0, 0, 0, 0}, 0.0f};
}

void Ball::move(const SDL_Rect &fieldBounds, const std::vector<std::pair<SDL_Rect, float>> &rect_list)
{
    // printf("Ball posx: %f, posy: %f\n", pos.x, pos.y);
    float max_speed = config::BALL_SPEED;
    auto p = collide(rect_list);
    auto rect = p.first;
    max_speed = (p.second > max_speed ? p.second : max_speed);
    // printf("max_speed: %f", max_speed);
    if (rect.w != 0 and rect.h != 0)
    {
        // printf("Collide rect");
        float closest_x = clamp(float(pos.x), float(rect.x), float(rect.x + rect.w));
        float closest_y = clamp(float(pos.y), float(rect.y) + float(rect.h) / 2, rect.y + float(rect.h));
        // printf("Closest point x: %f, y: %f\n", closest_x, closest_y);
        float dx = pos.x - closest_x;
        float dy = pos.y - closest_y;
        normalize_velocity(dx, dy, max_speed);
        // printf("dx: %f, dy: %f\n", dx, dy);
        vx = dx;
        vy = dy;
        // printf("Rect x: %d, y: %d\n", rect.x, rect.y);
    }
    if (vx == 0 && vy == 0)
    {
        return;
    }
    // printf("Ball vx: %f, vy: %f\n", vx, vy);
    // Direction handling (for goals or further boundary conditions)
    if (goal_checking())
    {
        pos.x = pos.x + vx;
        pos.y = pos.y + vy;
        return;
    }
    // Bounce off field boundaries
    if (pos.x - radius < fieldBounds.x || pos.x + radius > fieldBounds.x + fieldBounds.w)
    {
        if (!(pos.y > config::GOAL_TOP_Y and pos.y < config::GOAL_BOTTOM_Y))
        {
            vx = -vx;
            pos.x = clamp(pos.x, fieldBounds.x + radius, fieldBounds.x + fieldBounds.w - radius);
        }
    }
    if (pos.y - radius < fieldBounds.y || pos.y + radius > fieldBounds.y + fieldBounds.h)
    {
        vy = -vy;
        pos.y = clamp(pos.y, fieldBounds.y + radius, fieldBounds.y + fieldBounds.h - radius);
    }
    // printf("vx: %f, vy: %f\n", vx, vy);

    pos.x = pos.x + vx;
    pos.y = pos.y + vy;

    // printf("Ball later posx: %d, posy: %d\n", pos.x, pos.y);
    // Update position based on velocity and apply damping

    apply_friction(vx, vy, d_factor);
}

bool Ball::goal_checking()
{

    // Check for goals and update the ball position accordingly
    if (pos.y > config::GOAL_TOP_Y and pos.y < config::GOAL_BOTTOM_Y)
    {
        if (pos.x + radius + vx < config::GOAL_LEFT_X)
        {
            ingoal[0] = 1;
            return true;
        }
        else if (pos.x - radius + vx > config::GOAL_RIGHT_X)
        {
            ingoal[1] = 1;
            return true;
        }
    }
    return false;
}

int Ball::scoring()
{
    // printf("Ball posx: %f, posy: %f\n", pos.x, pos.y);
    // printf("Scoring");
    // Check if the ball is in goal area, reset ball and return scoring team
    if (ingoal[0])
    {
        pos = start_pos; // Reset ball position after goal
        ingoal[0] = 0;
        vx = 0; // Reset velocity
        vy = 0;
        return 0; // Team 1 scored
    }
    else if (ingoal[1])
    {
        pos = start_pos;
        ingoal[1] = 0;
        vx = 0;
        vy = 0;
        return 1; // Team 2 scored
    }
    return -1; // No goal
}

SDL_FPoint Ball::get_pos() const
{
    return pos;
}
