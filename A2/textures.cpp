#include "textures.hpp"

Texture::Texture() { textures.fill(nullptr); }

// Getter function with enum
SDL_Texture *Texture::get_home_message(TextureType type) const
{
    return textures[static_cast<size_t>(type)];
}

// Setter function with enum
void Texture::set_texture(TextureType type, SDL_Texture *texture)
{
    if (textures[static_cast<size_t>(type)])
    {
        SDL_DestroyTexture(textures[static_cast<size_t>(type)]);
        // printf("texture destroyed");
    }
    textures[static_cast<size_t>(type)] = texture;
}

// Load a texture from an image file
bool Texture::load_texture(TextureType type, const std::string &file_path, SDL_Renderer *renderer)
{
    if (!renderer)
    {
        printf("Renderer is null! Cannot load texture.\n");
        return false;
    }

    SDL_Surface *surface = SDL_LoadBMP(file_path.c_str()); // Load BMP image (or change to SDL_image for PNG)
    if (!surface)
    {
        printf("Failed to load image: %s, SDL_Error: %s\n", file_path.c_str(), SDL_GetError());
        return false;
    }

    if (type == TextureType::HOME_PLAYER or type == TextureType::AWAY_PLAYER)
    {
        load_player_ss(surface, renderer);
    }
    else if (type == TextureType::BALL_LEFT1)
    {
        load_ball_spritesheet(surface, renderer);
    }
    else
    {
        textures[static_cast<size_t>(type)] = SDL_CreateTextureFromSurface(renderer, surface);
        if (!textures[static_cast<size_t>(type)])
        {
            printf("Failed to create texture: %s, SDL_Error: %s\n", file_path.c_str(), SDL_GetError());
            return false;
        }
    }
    SDL_FreeSurface(surface); // Free the surface after creating the texture
    return true;
}
void Texture::load_ball_spritesheet(SDL_Surface *surface, SDL_Renderer *renderer)
{
    int sheetWidth = surface->w;
    int sheetHeight = surface->h;
    int spritesPerRow = sheetWidth / config::BALL_SS_WIDTH;
    int spritesPerColumn = sheetHeight / config::BALL_SS_HEIGHT;
    int totalSprites = spritesPerRow * spritesPerColumn;
    printf("total sprite: %d", totalSprites);
    for (int i = 0; i < totalSprites; i++)
    {
        SDL_Rect srcRect;
        srcRect.x = (i % spritesPerRow) * config::BALL_SS_WIDTH;
        srcRect.y = (i / spritesPerRow) * config::BALL_SS_HEIGHT;
        srcRect.w = config::BALL_SS_WIDTH;
        srcRect.h = config::BALL_SS_HEIGHT;

        SDL_Surface *spriteSurface = SDL_CreateRGBSurface(0, config::BALL_SS_WIDTH, config::BALL_SS_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        if (!spriteSurface)
        {
            std::cerr << "Failed to create sprite surface: " << SDL_GetError() << std::endl;
            continue;
        }

        SDL_BlitSurface(surface, &srcRect, spriteSurface, nullptr);

        SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);

        if (!spriteTexture)
        {
            std::cerr << "Failed to create texture for sprite " << i << ": " << SDL_GetError() << std::endl;
            continue;
        }
        textures[static_cast<size_t>(TextureType::BALL_RIGHT1) + i] = spriteTexture;
    }
}

std::vector<SDL_Texture *> Texture::get_sprite_list(TextureType type, int count) const
{
    std::vector<SDL_Texture *> spriteList;
    for (int i = 0; i < count; ++i)
    {
        SDL_Texture *texture = textures[static_cast<size_t>(type) + i];
        if (texture)
        {
            spriteList.push_back(texture);
        }
    }
    return spriteList;
}
void Texture::load_player_ss(SDL_Surface *surface, SDL_Renderer *renderer)
{
    int sheetWidth = surface->w;
    int sheetHeight = surface->h;
    int spritesPerRow = sheetWidth / config::SS_WIDTH;
    int spritesPerColumn = sheetHeight / config::SS_HEIGHT;
    int totalSprites = spritesPerRow * spritesPerColumn;
    for (int i = 0; i < totalSprites; i++)
    {
        SDL_Rect srcRect;
        srcRect.x = (i % spritesPerRow) * config::SS_WIDTH;
        srcRect.y = (i / spritesPerRow) * config::SS_HEIGHT;
        srcRect.w = config::SS_WIDTH;
        srcRect.h = config::SS_HEIGHT;

        SDL_Surface *spriteSurface = SDL_CreateRGBSurface(0, config::SS_WIDTH, config::SS_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        if (!spriteSurface)
        {
            std::cerr << "Failed to create sprite surface: " << SDL_GetError() << std::endl;
            continue;
        }

        // Copy pixels from the sprite sheet into the new surface
        SDL_BlitSurface(surface, &srcRect, spriteSurface, nullptr);

        // Convert the surface into a texture
        SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
        SDL_FreeSurface(spriteSurface);

        if (!spriteTexture)
        {
            std::cerr << "Failed to create texture for sprite " << i << ": " << SDL_GetError() << std::endl;
            continue;
        }
        textures[static_cast<size_t>(TextureType::HOME_PLAYER) + i] = spriteTexture;
        // Store each sprite as an individual texture
    }
}
// Load a texture from text (Requires SDL_ttf)
bool Texture::load_text_texture(TextureType type, const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
    auto texture = text_texture(message, fontFile, color, fontSize, renderer);
    if (!texture)
    {
        return false;
    }
    set_texture(type, texture);
    return true;
}
SDL_Rect get_texture_rect(SDL_Texture *t, SDL_Rect parent_rect, int maxwidth, int maxheight)
{
    int tw, th;
    SDL_QueryTexture(t, NULL, NULL, &tw, &th);
    float aspectRatio = (float)tw / (float)th;

    // Scale to fit within maxWidth and maxHeight while maintaining aspect ratio
    int renderW = maxwidth;
    int renderH = maxwidth / aspectRatio;

    if (renderH > maxheight)
    { // If height exceeds max, scale down
        renderH = maxheight;
        renderW = maxheight * aspectRatio;
    }
    // printf("rect width: %d, height: %d\n", renderW, renderH);
    int newx = (parent_rect.w - renderW) / 2 + parent_rect.x;
    int newy = (parent_rect.h - renderH) / 2 + parent_rect.y;

    SDL_Rect rect = {newx, newy, renderW, renderH};
    return rect;
}
// Get a texture
SDL_Texture *Texture::get_texture(TextureType type) const
{
    return textures[static_cast<size_t>(type)];
}

// Destructor to free textures
Texture::~Texture()
{
    for (SDL_Texture *texture : textures)
    {
        if (texture)
        {
            SDL_DestroyTexture(texture);
        }
    }
}