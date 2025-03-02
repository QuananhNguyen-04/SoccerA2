#include "config.hpp"

namespace config {
    float BALL_SPEED = config::BORDER.w / 250.0f;
    float KICK_SPEED = config::BORDER.w / 150.0f;
    float PLAYER_SPEED = config::BORDER.w / 290.0f;
    int FPS = 150;
    int FONT_SIZE = 50;
    std::string FONT_PATH = "./include/Roboto-Bold.ttf";
    std::string PLAYERSPRITE_PATH = "./assests/Players.bmp";
    std::string BALLSPRITE_PATH = "./assests/Ball.bmp";
}