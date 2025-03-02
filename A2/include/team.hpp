#ifndef TEAM_HPP
#define TEAM_HPP

#include "player.hpp"
#include "utils.hpp"
#include "textures.hpp"
#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <thread>

class Team
{
public:
    Team();
    Team(Texture *manager);
    void add_player(Player player);
    void new_team(std::vector<Player> p_list);
    void switch_player(bool team, SDL_FPoint ball);
    Player *get_current_p(bool team);
    void set_current_p(Player player);
    void move_up(bool team);
    void move_down(bool team);
    void move_left(bool team);
    void move_right(bool team);
    void move(SDL_FPoint ball);
    void init_pos(bool team);
    void score(bool team);
    void draw(SDL_Renderer *renderer);
    void kicking(bool team);
    void reset_after_timing(bool team);
    double closest(SDL_FPoint ball);
    std::vector<std::pair<SDL_Rect, float>> get_rect_list();
    void get_reward(double reward);
    void train();
    void save_weights();
    bool load_weights();

private:
    struct Contact
    {
        char key;
        uint16_t time;
    };
    std::vector<std::vector<double>> states;
    std::vector<std::vector<double>> next_states;
    std::vector<int> actions;
    std::vector<double> rewards;
    std::vector<bool> dones;
    std::vector<Player> team1, team2;
    int num;
    Player *current_p1, *current_p2;
    Texture *txtr_manager;
    Contact contact1, contact2;
    int score_1, score_2;
    int goal;
    SDL_Point start_pos;
    bool command1, command2;
};

#endif // TEAM_HPP
