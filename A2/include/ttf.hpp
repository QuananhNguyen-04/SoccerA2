#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

SDL_Texture* text_texture(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer);


#endif 