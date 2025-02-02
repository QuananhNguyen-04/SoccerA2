#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL2/SDL.h>
#include "ttf.hpp"
#include <cmath>
#include <array>

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

    bool load_text_texture(TextureType type, const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer);

    SDL_Texture *get_texture(TextureType type) const;

    // Destructor to free textures
    ~Texture();
};

#endif