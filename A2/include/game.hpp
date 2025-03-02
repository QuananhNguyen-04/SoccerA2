#define SDL_MAIN_HANDLED

#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "config.hpp"
#include "background.hpp"
#include "ball.hpp"
#include "player.hpp"
#include "team.hpp"
#include "ttf.hpp"
#include "textures.hpp"
#include <iostream>

class Game
{
private:
    /* data */
    Background bg;
    Texture txtr_manager;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    SDL_Surface *m_surface;
    Ball ball;
    Team teams;
    bool is_error;
public:
    Game(/* args */);
    ~Game();
    bool init();
    int run();
    void draw(uint32_t time_frame, bool drawing);
};

