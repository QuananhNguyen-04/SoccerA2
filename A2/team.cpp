// team.cpp
#include "team.hpp"

Team::Team()
{
    team1.clear();
    team2.clear();
    num = 5; // default number of players per team
    score_1 = score_2 = 0;
    goal = -1;
}

void Team::add_player(Player player)
{
    if (player.get_team())
    {
        team1.push_back(player);
    }
    else
    {
        team2.push_back(player);
    }
}

void Team::new_team(std::vector<Player> p_list)
{
    if (p_list[0].get_team())
    {
        team1 = p_list;

        current_p1 = &team1[0];
    }
    else
    {
        team2 = p_list;
        // for (auto p : p_list) {
        //     std::cout << &p << std::endl;
        // }
        current_p2 = &team2[0];
    }
    num = p_list.size();
}

void Team::switch_player(bool team, SDL_FPoint ball)
{
    if (num < 2) return;
    std::vector<Player *> temp;
    if (team == true)
    {
        for (auto &player : team1) {
            temp.push_back(&player);
        }
        // Switch player for team 1
        std::sort(temp.begin(), temp.end(), [&ball](Player a, Player b)
                    { return distance(
                        a.get_rect().x + a.get_rect().w / 2, a.get_rect().y + a.get_rect().h / 2, 
                        static_cast<float>(ball.x), static_cast<float>(ball.y)) <
                        distance(
                            b.get_rect().x + b.get_rect().w / 2, b.get_rect().y + b.get_rect().h / 2, 
                        static_cast<float>(ball.x), static_cast<float>(ball.y)); 
                    });
        if (current_p1 != temp[0]) current_p1 = temp[0];
        else current_p1 = temp[1];
    }
    else if (team == false)
    {
        for (auto &player : team2) {
            temp.push_back(&player);
        }
        std::sort(temp.begin(), temp.end(), [&ball](Player a, Player b)
                    { return distance(
                        a.get_rect().x + a.get_rect().w / 2, a.get_rect().y + a.get_rect().h / 2, 
                        static_cast<float>(ball.x), static_cast<float>(ball.y)) <
                        distance(
                            b.get_rect().x + b.get_rect().w / 2, b.get_rect().y + b.get_rect().h / 2, 
                        static_cast<float>(ball.x), static_cast<float>(ball.y)); 
                    });
        if (current_p2 != temp[0]) current_p2 = temp[0];
        else current_p2 = temp[1];
        // Switch player for team 2
    }
}

Player *Team::get_current_p(bool team)
{
    if (team)
    {
        return current_p1;
    }
    else
    {
        return current_p2;
    }
}

void Team::score(bool team)
{
    if (team)
    {
        score_1++;
    }
    else
    {
        score_2++;
    }
}

void Team::move_up(bool team)
{
    if (team)
    {
        current_p1->move_up();
    }
    else
    {
        current_p2->move_up();
    }
}
void Team::move_down(bool team)
{
    if (team)
    {
        current_p1->move_down();
    }
    else
    {
        current_p2->move_down();
    }
}
void Team::move_left(bool team)
{
    if (team)
    {
        current_p1->move_left();
    }
    else
    {
        current_p2->move_left();
    }
}
void Team::move_right(bool team)
{
    if (team)
    {
        current_p1->move_right();
    }
    else
    {
        current_p2->move_right();
    }
}

void Team::move()
{
    // std::cout << "team1\n";
    for (auto &player : team1)
    {

        player.move();
    }

    // std::cout << "team2\n";
    for (auto &player : team2)
    {

        player.move();
    }
}

void Team::init_pos()
{
    // Reset positions after a goal
}

void Team::draw(SDL_Renderer *renderer)
{
    for (auto &player : team1)
    {
        player.draw(renderer);
    }
    for (auto &player : team2)
    {
        player.draw(renderer);
    }
}

std::vector<SDL_Rect> Team::get_rect_list()
{
    SDL_assert(team1.size() > 0);
    SDL_assert(team1.size() == team2.size());
    SDL_assert(team1.size() == num);
    std::vector<SDL_Rect> rect_list(2 * num);
    for (int i = 0; i < num; ++i)
    {
        rect_list[i] = team1[i].get_rect();
    }
    for (int i = 0; i < num; ++i)
    {
        rect_list[i + num] = team2[i].get_rect();
    }
    return rect_list;
}
