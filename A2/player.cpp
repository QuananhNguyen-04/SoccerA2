// player.cpp
#include "player.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player() {}

Player::Player(bool team) : team(team), vx(0), vy(0)
{
    rect = SDL_Rect{100, 100, config::PLAYER_WIDTH, config::PLAYER_HEIGHT}; // Example size
    df = config::PLAYER_DAMPING_FACTOR;
    kick = 0.0f;
    field_bounds = config::BORDER;
}

void Player::set_pos(int x, int y)
{
    rect.x = x;
    rect.y = y;
}

void Player::move_left()
{
    vx = -config::PLAYER_SPEED;
}

void Player::move_right()
{
    vx = config::PLAYER_SPEED;
}

void Player::move_up()
{
    vy = -config::PLAYER_SPEED;
}

void Player::move_down()
{
    vy = config::PLAYER_SPEED;
}

void Player::move()
{
    // std::cout << this << " " << std::endl;
    if (vx == 0 && vy == 0)
    {
        return;
    }
    normalize_velocity(vx, vy, config::PLAYER_SPEED);
    if (rect.x + vx < field_bounds.x || rect.x + rect.w + vx > field_bounds.x + field_bounds.w)
    {
        vx = 0;
        // rect.x = clamp(rect.x, field_bounds.x + rect.w / 2, field_bounds.x + field_bounds.w - rect.w / 2);
    }
    if (rect.y + rect.h / 2 + vy < field_bounds.y || rect.y + rect.h + vy > field_bounds.y + field_bounds.h)
    {
        vy = 0;
        // rect.y = clamp(rect.y, field_bounds.y + rect.h / 2, field_bounds.y + field_bounds.h - rect.h / 2);
    }
    if (vx == 0 && vy == 0)
    {
        return;
    }
    rect.x = std::round(rect.x + vx);
    rect.y = std::round(rect.y + vy);
    printf("Player vx: %f, vy: %f\n", vx, vy);
    // printf("Player posx: %d, posy: %d\n", rect.x, rect.y);
    apply_friction(vx, vy, df);
}

void Player::draw(SDL_Renderer *renderer)
{
    if (team)
    {
        SDL_SetRenderDrawColor(renderer, config::HomeColor.r, config::HomeColor.g, config::HomeColor.b, 255); // Red color for player
        SDL_RenderFillRect(renderer, &rect);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, config::AwayColor.r, config::AwayColor.g, config::AwayColor.b, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Player::get_team() const
{
    return team;
}

float Player::get_kick() const
{
    return kick;
}
void Player::set_kick(bool flag)
{
    if (flag)
    {
        kick = 0.0f;
        return;
    }
    if (kick != 0.0f)
    {
        kick = 0.0f;
    }
    else
    {
        kick = config::KICK_SPEED;
    }
}
SDL_Rect Player::get_rect() const
{
    return rect;
}

PlayerFactory::PlayerFactory() {}

std::vector<Player> PlayerFactory::create(int num, bool team)
{
    std::vector<Player> players;
    for (int i = 0; i < num; ++i)
    {
        players.push_back(Player(team));
        if (!team)
            players.back().set_pos(config::BORDER.w - 100, config::BORDER.h - 100);
    }
    return players;
}