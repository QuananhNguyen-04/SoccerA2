// background.cpp
#include "background.hpp"
#include "config.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "textures.hpp"

using config::GREEN;
Background::Background(Texture *manager,
                       SDL_Window *m_window,
                       SDL_Renderer *m_renderer) : m_window(m_window),
                                                   m_renderer(m_renderer),
                                                   txtr_manager(manager)
{

    m_rect = {0, 0, config::WINDOW.w, config::WINDOW.h};
    m_score[0] = 0;
    m_score[1] = 0;
}

void Background::draw_thick_line(SDL_Rect rect, int bold)
{
    SDL_SetRenderDrawColor(m_renderer, config::WHITE.r, config::WHITE.g, config::WHITE.b, 255);
    for (int i = -bold / 2; i < bold / 2; i++)
    {
        SDL_Rect temp_rect = {rect.x + i, rect.y + i, rect.w - 2 * i, rect.h - 2 * i};
        SDL_RenderDrawRect(m_renderer, &temp_rect);
    }
}
void Background::draw_score()
{
    SDL_Rect m_windowrect = {0, 0, 0, 0};
    SDL_GetWindowSize(m_window, &m_windowrect.w, &m_windowrect.h);
    SDL_SetRenderDrawColor(m_renderer, config::LIGHT_GRAY.r, config::LIGHT_GRAY.g, config::LIGHT_GRAY.b, 255);
    SDL_RenderFillRect(m_renderer, &m_windowrect);

    SDL_Rect home_dst_rect = get_texture_rect(
        txtr_manager->get_texture(TextureType::HOME_MESSAGE),
        (SDL_Rect){config::WINDOW.w, 100, config::SCREEN_WIDTH - config::WINDOW.w},
        300, 100);
    SDL_RenderCopy(m_renderer, txtr_manager->get_texture(TextureType::HOME_MESSAGE), NULL, &home_dst_rect); // Render the text

    SDL_Rect homescore_dst_rect = get_texture_rect(
        txtr_manager->get_texture(TextureType::HOME_SCORE),
        (SDL_Rect){config::WINDOW.w, 200, config::SCREEN_WIDTH - config::WINDOW.w, 100},
        300, 100);
    SDL_RenderCopy(m_renderer, txtr_manager->get_texture(TextureType::HOME_SCORE), NULL, &homescore_dst_rect); // Render the text

    SDL_Rect away_dst_rect = get_texture_rect(
        txtr_manager->get_texture(TextureType::AWAY_MESSAGE),
        (SDL_Rect){config::WINDOW.w, 400, config::SCREEN_WIDTH - config::WINDOW.w},
        300, 100);
    SDL_RenderCopy(m_renderer, txtr_manager->get_texture(TextureType::AWAY_MESSAGE), NULL, &away_dst_rect); // Render the text

    SDL_Rect awayscore_dst_rect = get_texture_rect(
        txtr_manager->get_texture(TextureType::AWAY_SCORE),
        (SDL_Rect){config::WINDOW.w, 500, config::SCREEN_WIDTH - config::WINDOW.w, 100},
        300, 100);
    SDL_RenderCopy(m_renderer, txtr_manager->get_texture(TextureType::AWAY_SCORE), NULL, &awayscore_dst_rect); // Render the text
}
void Background::draw(uint32_t current_time)
{
    // Get the current time in the format ab:cd
    // time_t now = time(0);
    // tm *ltm = localtime(&now);

    // 45 000 -> 45m 00s
    // 20 400 -> 20m 400/1000*60s
    current_time -= 1000 * (m_score[0] + m_score[1]);
    uint32_t minutes = (current_time / 1000) % 60;
    uint32_t seconds = (current_time % 1000) / 1000.0 * 60;

    std::string current_time_str = (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);

    // Create a texture for the current time
    txtr_manager->load_text_texture(TextureType::TIMER, current_time_str, config::FONT_PATH, config::WHITE, config::FONT_SIZE, m_renderer);

    // Render the current time texture
    SDL_SetRenderDrawColor(m_renderer, config::LIGHT_GRAY.r, config::LIGHT_GRAY.g, config::LIGHT_GRAY.b, 255);
    SDL_Rect time_dst_rect = get_texture_rect(
        txtr_manager->get_texture(TextureType::TIMER),
        (SDL_Rect){config::WINDOW.w, 700, config::SCREEN_WIDTH - config::WINDOW.w, 100},
        300, 100);
    SDL_RenderFillRect(m_renderer, &time_dst_rect);
    SDL_RenderCopy(m_renderer, txtr_manager->get_texture(TextureType::TIMER), NULL, &time_dst_rect);
    
    auto border = config::BORDER;
    SDL_SetRenderDrawColor(m_renderer, config::GREEN.r, config::GREEN.g, config::GREEN.b, 255);
    SDL_RenderFillRect(m_renderer, &m_rect);
    SDL_SetRenderDrawColor(m_renderer, config::WHITE.r, config::WHITE.g, config::WHITE.b, 255);
    SDL_RenderDrawRect(m_renderer, &border);
    draw_thick_line(border, 7);
    SDL_Rect left_goal = {config::GOAL_LEFT_X - config::GOAL_DEPTH, config::GOAL_TOP_Y, config::GOAL_DEPTH, config::GOAL_WIDTH};
    SDL_Rect right_goal = {config::GOAL_RIGHT_X, config::GOAL_TOP_Y, config::GOAL_DEPTH, config::GOAL_WIDTH};
    draw_thick_line(left_goal, 5);
    draw_thick_line(right_goal, 5);
}

void Background::update_score_home() // home scoring
{
    goal();
    m_score[0] += 1;
    txtr_manager->load_text_texture(TextureType::HOME_SCORE, std::to_string(m_score[0]), config::FONT_PATH, config::HomeColor, config::FONT_SIZE, m_renderer);
    printf("delayyyy, score ratio: %d : %d", m_score[0], m_score[1]);
    SDL_Delay(2000);
    draw_score();
}
void Background::update_score_away() 
{
    // # away scoring
    goal();
    m_score[1] += 1;
    txtr_manager->load_text_texture(TextureType::AWAY_SCORE, std::to_string(m_score[1]), config::FONT_PATH, config::AwayColor, config::FONT_SIZE, m_renderer);
    printf("delayyyy");
    SDL_Delay(2000);
    draw_score();
}
void Background::goal()
{
    SDL_Rect goal_dst_rect = get_texture_rect(txtr_manager->get_texture(TextureType::GOAL_MESSAGE), config::WINDOW, 500, 300);
    SDL_RenderCopy(m_renderer, txtr_manager->get_texture(TextureType::GOAL_MESSAGE), NULL, &goal_dst_rect);
    SDL_RenderPresent(m_renderer);
}

void Background::reset_layout()
{
    m_score[0] = 0;
    m_score[1] = 0;
    // SDL_Delay(2000);
    draw_score();
}

SDL_Renderer *Background::get_renderer()
{
    return m_renderer;
}

Background::Background() {}