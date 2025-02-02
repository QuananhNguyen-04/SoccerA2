#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SDL2/SDL.h>
#include "config.hpp"
#include "textures.hpp"

class Background
{
public:
    Background();
    Background(Texture *manager, SDL_Window *m_window,
               SDL_Renderer *m_renderer);
    void draw();
    void draw_thick_line(SDL_Rect rect, int bold);
    void draw_score();
    void goal();
    void reset_layout();
    void update_score_home();
    void update_score_away();
    // void draw_text(std::string text, int font_size, int x, int y);
    SDL_Renderer *get_renderer();

private:
    Texture *txtr_manager;
    int m_score[2];
    SDL_Renderer *m_renderer;
    SDL_Window *m_window;
    SDL_Rect m_rect;
};

#endif // BACKGROUND_HPP
