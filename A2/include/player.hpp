#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include "config.hpp"
#include "agent.hpp"
#include <vector>
class Player {
public:
    Player();
    Player(bool team, const std::vector<int> &layers, const std::vector<int> &vlayers);
    void set_pos(int x, int y);
    void move_left();
    void move_right();
    void move_up();
    void move_down();
    void move();
    void draw(SDL_Texture* texture, SDL_Renderer* renderer);
    bool get_team() const;
    SDL_FPoint get_vel() const;
    float get_kick() const;
    void set_kick(bool flag = 0);
    SDL_Rect get_rect() const;
    Agent& getAgent();
    void is_controlled();
    bool controlled;
private:
    Agent agent;
    float kick;
    SDL_Rect field_bounds;
    float df;
    SDL_Rect rect;
    bool team;
    SDL_Color color;
    float vx, vy;
};

class PlayerFactory {
public:
    PlayerFactory();
    std::vector<Player> create(int num, bool team);
};

#endif // PLAYER_HPP