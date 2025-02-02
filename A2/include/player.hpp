#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL2/SDL.h>
#include "config.hpp"
#include <vector>
class Player {
public:
    Player();
    Player(bool team);
    void set_pos(int x, int y);
    void move_left();
    void move_right();
    void move_up();
    void move_down();
    void move();
    void draw(SDL_Renderer* renderer);
    bool get_team() const;
    SDL_Rect get_rect() const;
private:
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