#ifndef BALL_HPP
#define BALL_HPP

#include <SDL2/SDL.h>
#include "textures.hpp"
#include <vector>

class Ball {
public:
    Ball();
    Ball(Texture * manager, SDL_Renderer* renderer);
    void draw();
    SDL_Rect collide(const std::vector<SDL_Rect>& rect_list);

    void special(const char& key);
    void move(const SDL_Rect& fieldBounds, const std::vector<SDL_Rect>& rect_list);
    bool goal_checking();
    // void direction_handling();
    int scoring();
    SDL_FPoint get_pos() const;
private:
    Texture * manager;
    SDL_Renderer* renderer;
    int radius;
    SDL_FPoint start_pos;
    SDL_FPoint pos;
    float vx, vy;
    float d_factor;
    bool ingoal[3];
};

#endif // BALL_HPP
