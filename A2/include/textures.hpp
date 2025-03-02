#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL.h>
#include "ttf.hpp"
#include <config.hpp>
#include <cmath>
#include <array>
#include <vector>

enum class TextureType
{
    HOME_MESSAGE,
    AWAY_MESSAGE,
    HOME_SCORE,
    AWAY_SCORE,
    GOAL_MESSAGE,
    GOAL_RATIO,
    HOME_MESSAGE7,
    HOME_MESSAGE8,
    HOME_PLAYER, 
    AWAY_PLAYER,
    BALL_RIGHT1,
    BALL_RIGHT2,
    BALL_RIGHT3,
    BALL_RIGHT4,
    BALL_LEFT1,
    BALL_LEFT2,
    BALL_LEFT3,
    BALL_LEFT4,
    TIMER,
    COUNT // Always keep this last, it represents the number of textures
};

SDL_Rect get_texture_rect(SDL_Texture* t, SDL_Rect parent_rect, int maxwidth, int maxheight);
class Texture
{
private:
    std::array<SDL_Texture *, static_cast<size_t>(TextureType::COUNT)> textures;

public:
    // Constructor initializes all textures to nullptr
    Texture();

    // Getter function with enum
    SDL_Texture *get_home_message(TextureType type) const;

    // Setter function with enum
    void set_texture(TextureType type, SDL_Texture *texture);

    bool load_texture(TextureType type, const std::string &file_path, SDL_Renderer * renderer);

    void load_ball_spritesheet(SDL_Surface *surface, SDL_Renderer *renderer);

    std::vector<SDL_Texture *> get_sprite_list(TextureType type, int count) const;

    void load_player_ss(SDL_Surface *surface, SDL_Renderer *renderer);

    bool load_text_texture(TextureType type, const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer);

    SDL_Texture *get_texture(TextureType type) const;

    // Destructor to free textures
    ~Texture();
};

#endif