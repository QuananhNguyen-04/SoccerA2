// player.cpp
#include "player.hpp"
#include "utils.hpp"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player(): agent({1,2,1}, {1,2,1}) {
}

Player::Player(bool team, const std::vector<int>& layers, const std::vector<int>& vlayers ) : team(team), vx(0), vy(0), agent(layers, vlayers)
{
    std::cout << "vlayers shape: ";
    for (const auto &layer : vlayers)
    {
        std::cout << layer << " ";
    }
    std::cout << std::endl;
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
    // printf("Player vx: %f, vy: %f\n", vx, vy);
    // printf("Player posx: %d, posy: %d\n", rect.x, rect.y);
    apply_friction(vx, vy, df);
}

void Player::draw(SDL_Texture* texture, SDL_Renderer *renderer)
{   
    auto draw_rect = rect;

    draw_rect.w = config::DRAW_PLAYER_WIDTH;
    draw_rect.h = config::DRAW_PLAYER_HEIGHT;
    draw_rect.x = rect.x + (rect.w - draw_rect.w) / 2;
    draw_rect.y = rect.y + rect.h - draw_rect.h;
    if (team)
    {
        SDL_RenderCopy(renderer, texture, NULL, &draw_rect);
        // SDL_SetRenderDrawColor(renderer, config::HomeColor.r, config::HomeColor.g, config::HomeColor.b, 255); // Red color for player
        // SDL_RenderFillRect(renderer, &draw_rect);
    }
    else
    {
        SDL_RenderCopy(renderer, texture, NULL, &draw_rect);
        // SDL_SetRenderDrawColor(renderer, config::AwayColor.r, config::AwayColor.g, config::AwayColor.b, 255);
        // SDL_RenderFillRect(renderer, &draw_rect);
    }
}

bool Player::get_team() const
{
    return team;
}

SDL_FPoint Player::get_vel() const
{
    return SDL_FPoint({vx, vy});
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

Agent &Player::getAgent()
{
    return agent; 
}

void Player::is_controlled()
{
    controlled = !controlled;
}

PlayerFactory::PlayerFactory() {}

std::vector<Player> PlayerFactory::create(int num, bool team)
{
    int input = config::NUM_PLAYERS_PER_TEAM * 2 * 2 + 2; // coordination of each player and ball
    int output = 4; // moving and kick
    std::vector<Player> players;
    std::vector<int> layers(5);
    layers[0] = input;
    layers[1] = 6 * input;
    layers[2] = 4 * input;
    layers[3] = 2 * output;
    layers[4] = output;
    auto vlayers = layers;
    vlayers.back() = 1;
    int padding = (config::BORDER.h - 100) / config::NUM_PLAYERS_PER_TEAM;
    for (int i = 0; i < num; ++i)
    {
        players.push_back(Player(team, layers, vlayers));
        if (team) 
            players.back().set_pos(100, 100 + padding * i);
        if (!team)
            players.back().set_pos(config::BORDER.w - 100, config::BORDER.h - 100 - padding * i);
    }
    return players;
}