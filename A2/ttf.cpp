#include "ttf.hpp"

SDL_Texture* text_texture(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer) {
    // Load the font
    TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << "\n";
        return nullptr;
    }

    // Render text to a surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, message.c_str(), color);
    if (!textSurface) {
        std::cerr << "Unable to render text surface! TTF_Error: " << TTF_GetError() << "\n";
        TTF_CloseFont(font);
        return nullptr;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create texture from text surface! SDL_Error: " << SDL_GetError() << "\n";
    }

    // Clean up surface and font
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    return textTexture;
}