#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SDL2/SDL.h>
#include <cmath>
#include <string>
// #include <stdio.h>
namespace config {

    // Game window dimensions
    const int SCREEN_WIDTH = 1500;
    const int SCREEN_HEIGHT = 900;
    
    // Color definitions
    const SDL_Color GREEN = {0, 255, 0, 255};   // Green color
    const SDL_Color WHITE = {255, 255, 255, 255}; // White color
    const SDL_Color LIGHT_GRAY = {100, 100, 100, 255}; // Light gray
    const SDL_Color RED = {255, 0, 0, 255}; // Light gray
    const SDL_Color BLUE = {0, 0, 255, 255}; // Light gray

    // Player properties
    const int PLAYER_WIDTH = 20;
    const int PLAYER_HEIGHT = 50;
    const SDL_Color HomeColor = RED;
    const SDL_Color AwayColor = BLUE;


    // Ball properties
    const int BALL_RADIUS = 15;   // Radius of the ball
    const float BALL_DAMPING_FACTOR = 0.97f; // Damping factor for the ball's velocity
    const float PLAYER_DAMPING_FACTOR = 0.4f; // Damping factor for the player's velocity
    
    // Kicking and passing speeds
    extern float KICK_SPEED;
    extern float PASS_SPEED;
    extern float BALL_SPEED;
    extern float PLAYER_SPEED;

    // Font settings
    extern int FONT_SIZE;
    extern std::string FONT_PATH;

    // Rectangular area for the playing field (specific area within the window)
    const int BOUNDARY = 50;
    const SDL_Rect WINDOW = {0, 0, 1200, SCREEN_HEIGHT}; // x, y, width, height
    const SDL_Rect BORDER = {BOUNDARY, BOUNDARY, WINDOW.w - 2 * BOUNDARY, WINDOW.h - 2* BOUNDARY}; // x, y, width, height
    const int GOAL_WIDTH = 10 * BALL_RADIUS;
    const int GOAL_LEFT_X = BORDER.x;
    const int GOAL_RIGHT_X = BORDER.w + BORDER.x;
    const int GOAL_DEPTH = 20;
    const int GOAL_TOP_Y = BORDER.y + BORDER.h / 2 - GOAL_WIDTH / 2;
    const int GOAL_BOTTOM_Y = BORDER.y + BORDER.h / 2 + GOAL_WIDTH / 2;
    
    // Number of players per team
    const int NUM_PLAYERS_PER_TEAM = 5;

    const int FPS = 120;
    const int ACTION_TIME_LIMIT = 10; // in milliseconds
    
} // namespace config

#endif // CONFIG_HPP