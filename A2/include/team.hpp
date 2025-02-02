#ifndef TEAM_HPP
#define TEAM_HPP

#include "player.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
class Team {
public:
    Team();
    void add_player(Player player);
    void new_team(std::vector<Player> p_list);
    void switch_player(bool team, SDL_FPoint ball);
    Player* get_current_p(bool team);
    void set_current_p(Player player);
    void move_up(bool team);
    void move_down(bool team);
    void move_left(bool team);
    void move_right(bool team);
    void move();
    void init_pos();
    void score(bool team);
    void draw(SDL_Renderer* renderer);
    void passing(bool team);
    void kicking(bool team);
    void reset_after_timing();
    std::vector<SDL_Rect> get_rect_list();
private:
    struct Contact {
        char key;
        uint16_t time;
    };
    std::vector<Player> team1, team2;
    int num;
    Player *current_p1, *current_p2;
    
    Contact contact1, contact2;
    int score_1, score_2;
    int goal;
    SDL_Point start_pos;
    bool command1, command2;
};

#endif // TEAM_HPP
