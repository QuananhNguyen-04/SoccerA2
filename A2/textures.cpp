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
    if (textures[static_cast<size_t>(type)]) {
        SDL_DestroyTexture(textures[static_cast<size_t>(type)]);
        printf("texture destroyed");
    }
    textures[static_cast<size_t>(type)] = texture;
}

// Load a texture from an image file
bool Texture::load_texture(TextureType type, const std::string &file_path, SDL_Renderer * renderer)
{
    SDL_Surface *surface = SDL_LoadBMP(file_path.c_str()); // Load BMP image (or change to SDL_image for PNG)
    if (!surface)
    {
        printf("Failed to load image: %s, SDL_Error: %s\n", file_path.c_str(), SDL_GetError());
        return false;
    }

    textures[static_cast<size_t>(type)] = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Free the surface after creating the texture

    if (!textures[static_cast<size_t>(type)])
    {
        printf("Failed to create texture: %s, SDL_Error: %s\n", file_path.c_str(), SDL_GetError());
        return false;
    }

    return true;
}

// Load a texture from text (Requires SDL_ttf)
bool Texture::load_text_texture(TextureType type, const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer)
{
    auto texture = text_texture(message, fontFile, color, fontSize, renderer);
    if (!texture) {
        return false;
    }
    set_texture(type, texture);
    return true;
}
SDL_Rect get_texture_rect(SDL_Texture* t, SDL_Rect parent_rect, int maxwidth, int maxheight) {
    int tw, th;
    SDL_QueryTexture(t, NULL, NULL, &tw, &th);
    printf("textures width: %d, textures height: %d\n", tw, th);
    float aspectRatio = (float)tw / (float)th;
    
    // Scale to fit within maxWidth and maxHeight while maintaining aspect ratio
    int renderW = maxwidth;
    int renderH = maxwidth / aspectRatio;

    if (renderH > maxheight) {  // If height exceeds max, scale down
        renderH = maxheight;
        renderW = maxheight * aspectRatio;
    }
    printf("rect width: %d, height: %d\n", renderW, renderH);
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