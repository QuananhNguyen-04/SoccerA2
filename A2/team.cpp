// team.cpp
#include "team.hpp"

Team::Team()
{
}

Team::Team(Texture *manager) : txtr_manager(manager)
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
    if (num < 2)
        return;
    std::vector<Player *> temp;
    printf("length of team 1, 2: %d, %d", team1.size(), team2.size());

    if (team == true)
    {
        for (auto &player : team1)
        {
            temp.push_back(&player);
        }
        // Switch player for team 1
        std::sort(temp.begin(), temp.end(), [&ball](const Player *a, const Player *b)
                  { return distance(
                               a->get_rect().x + a->get_rect().w / 2, a->get_rect().y + a->get_rect().h / 2,
                               ball.x, ball.y) <
                           distance(
                               b->get_rect().x + b->get_rect().w / 2, b->get_rect().y + b->get_rect().h / 2,
                               ball.x, ball.y); });
        if (current_p1 != temp[0])
            current_p1 = temp[0];
        else
            current_p1 = temp[1];
    }
    else if (team == false)
    {
        for (auto &player : team2)
        {
            temp.push_back(&player);
        }
        std::sort(temp.begin(), temp.end(), [&ball](const Player *a, const Player *b)
                  { return distance(
                               a->get_rect().x + a->get_rect().w / 2, a->get_rect().y + a->get_rect().h / 2,
                               ball.x, ball.y) <
                           distance(
                               b->get_rect().x + b->get_rect().w / 2, b->get_rect().y + b->get_rect().h / 2,
                               ball.x, ball.y); });
        if (current_p2 != temp[0])
            current_p2 = temp[0];
        else
            current_p2 = temp[1];
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
        current_p1->is_controlled();
    }
    else
    {
        current_p2->move_up();
        current_p2->is_controlled();
    }
}
void Team::move_down(bool team)
{
    if (team)
    {
        current_p1->move_down();
        current_p1->is_controlled();
    }
    else
    {
        current_p2->move_down();
        current_p2->is_controlled();
    }
}
void Team::move_left(bool team)
{
    if (team)
    {
        current_p1->move_left();
        current_p1->is_controlled();
    }
    else
    {
        current_p2->move_left();
        current_p2->is_controlled();
    }
}
void Team::move_right(bool team)
{
    if (team)
    {
        current_p1->move_right();
        current_p1->is_controlled();
    }
    else
    {
        current_p2->move_right();
        current_p2->is_controlled();
    }
}

void Team::move(SDL_FPoint ball)
{

    // std::vector<double> env(config::NUM_PLAYERS_PER_TEAM * 4 + 2);
    SDL_assert(num == config::NUM_PLAYERS_PER_TEAM);
    const double X_min = config::BORDER.x;
    const double Y_min = config::BORDER.y;
    const double X_max = config::BORDER.w + config::BORDER.x;
    const double Y_max = config::BORDER.h + config::BORDER.y;
    // std::cout << "team1\n";
    std::vector<double> env(num * 4 + 2);
    env[num * 4] = (ball.x - X_min) / (X_max - X_min);
    env[num * 4 + 1] = (ball.y - Y_min) / (Y_max - Y_min);
    int idx = 0;
    for (int i = 0; i < num; i++)
    {
        env[2 * i + num] = (team2[i].get_rect().x - X_min) / (X_max - X_min);
        env[2 * i + num] = (team2[i].get_rect().y - Y_min) / (Y_max - Y_min);
    }
    for (auto &player : team1)
    {
        for (int i = 0; i < num; ++i)
        {
            // num 3 idx = 1
            // 2 3 1
            // 3 + 0 - 1 = 2
            // 3 + 1 - 1 = 3 -> 0
            // 3 + 2 - 1 = 4 -> 1
            env[2 * i] = (team1[(num + i - idx) % num].get_rect().x - X_min) / (X_max - X_min);
            env[2 * i + 1] = (team1[(num + i - idx) % num].get_rect().y - Y_min) / (Y_max - Y_min);
        }
        states.push_back(env);
        next_states.push_back(env);
        if ((abs(current_p1->get_vel().x) >= 1e-5 or abs(current_p1->get_vel().y) >= 1e-5) and current_p1 == &player)
        {
            // player.getAgent().policyNetwork.printWeights();
            actions.push_back(rand() % 4);
        }
        else
        {
            std::vector<int> action = player.getAgent().takeAction(env);
            auto self_reward = player.getAgent().takeValue(env);

            std::cout << "self_rewards: " << self_reward[0] << " ";
            int move_action = static_cast<int>(action[0]);
            actions.push_back(move_action);
            std::cout << "act1: " << move_action << " ";
            switch (move_action)
            {
            case 0:
                player.move_up();
                break;
            case 1:
                player.move_down();
                break;
            case 2:
                player.move_left();
                break;
            case 3:
                player.move_right();
                break;
            }
        }
        idx++;
        player.move();
    }
    std::cout << "\n";

    // std::cout << "team2\n";
    idx = 0;
    for (int i = 0; i < num; ++i)
    {
        env[2 * i] = (team1[i].get_rect().x - X_min) / (X_max - X_min);
        env[2 * i + 1] = (team1[i].get_rect().y - Y_min) / (Y_max - Y_min);
    }
    for (auto &player : team2)
    {
        for (int i = 0; i < num; ++i)
        {
            env[2 * i + num] = (team2[(num + i - idx) % num].get_rect().x - X_min) / (X_max - X_min);
            env[2 * i + num + 1] = (team2[(num + i - idx) % num].get_rect().y - Y_min) / (Y_max - Y_min);
        }
        states.push_back(env);
        next_states.push_back(env);
        if ((abs(current_p2->get_vel().x) >= 1e-5 or abs(current_p2->get_vel().y) >= 1e-5) and current_p2 == &player)
        {
            actions.push_back(rand() % 4);
        }
        else
        {

            std::vector<int> action = player.getAgent().takeAction(env);
            auto self_reward = player.getAgent().takeValue(env);
            std::cout << "self_rewards: " << self_reward[0] << " ";
            int move_action = static_cast<int>(action[0]);
            actions.push_back(move_action);
            std::cout << "act2: " << move_action << " ";
            switch (move_action)
            {
            case 0:
                player.move_up();
                break;
            case 1:
                player.move_down();
                break;
            case 2:
                player.move_left();
                break;
            case 3:
                player.move_right();
                break;
            }
        }
        idx++;

        player.move();
    }
    std::cout << std::endl;
}

void Team::init_pos(bool team)
{
    SDL_FPoint ball{config::BORDER.x + (float)config::BORDER.w / 2,
                    config::BORDER.y + (float)config::BORDER.h / 2};
    int padding = (config::BORDER.h - 100) / config::NUM_PLAYERS_PER_TEAM;
    // Reset positions after a goal
    for (int i = 0; i < num; ++i)
    {
        team1[i].set_pos(100 + rand() % 200, 100 + padding * i);
    }
    for (int i = 0; i < num; ++i)
    {
        team2[i].set_pos(config::BORDER.w - 100 - rand() % 200, config::BORDER.h - 100 - padding * i);
    }
    if (team)
    {
        current_p2->set_pos(ball.x - 100, config::BORDER.h / 2);
    }
    else
    {
        current_p1->set_pos(ball.x + 100, config::BORDER.h / 2);
    }
}

void Team::draw(SDL_Renderer *renderer)
{
    auto home_txtr = txtr_manager->get_texture(TextureType::HOME_PLAYER);
    auto away_txtr = txtr_manager->get_texture(TextureType::AWAY_PLAYER);
    for (auto &player : team1)
    {
        player.draw(home_txtr, renderer);
    }
    for (auto &player : team2)
    {
        player.draw(away_txtr, renderer);
    }
}

std::vector<std::pair<SDL_Rect, float>> Team::get_rect_list()
{
    SDL_assert(team1.size() > 0);
    SDL_assert(team1.size() == team2.size());
    SDL_assert(team1.size() == num);
    std::vector<std::pair<SDL_Rect, float>> rect_list(2 * num);
    for (int i = 0; i < num; ++i)
    {
        rect_list[i] = {team1[i].get_rect(), team1[i].get_kick()};
    }
    for (int i = 0; i < num; ++i)
    {
        rect_list[i + num] = {team2[i].get_rect(), team2[i].get_kick()};
    }
    return rect_list;
}

void Team::get_reward(double reward)
{
    for (int i = 0; i < 2 * num; ++i)
    {
        if (i < num)
            rewards.push_back(reward);
        else
            rewards.push_back(-reward);
        if (abs(reward) < 1)
            dones.push_back(false);
        else
            dones.push_back(true);
    }
}

void Team::train()
{
    if (states.size() < 200 * 2 * num)
        return;
    next_states.erase(next_states.end() - 2 * num, next_states.end());
    states.erase(states.begin(), states.begin() + 2 * num);
    rewards.erase(rewards.begin(), rewards.begin() + 2 * num);
    dones.erase(dones.begin(), dones.begin() + 2 * num);
    actions.erase(actions.begin(), actions.begin() + 2 * num);
    // printf("States size: %zu\n", states.size());
    // printf("Next states size: %zu\n", next_states.size());
    // printf("Rewards size: %zu\n", rewards.size());
    // printf("Dones size: %zu\n", dones.size());
    /*
        A A A A A    A A A A A
        B B B B B    B B B B B
        -C C C C C    C C C C C-

        r r r r r    r r r r r
        r r r r r    r r r r r

        d d d d d    d d d d d
        d d d d d    d d d d d

    */

    int num_players = 2 * num;
    size_t total_samples = states.size(); // Should be a multiple of num_players
    std::vector<std::vector<std::vector<double>>> states_team1(num), states_team2(num);
    std::vector<std::vector<double>> rewards_team1(num), rewards_team2(num);
    std::vector<std::vector<std::vector<double>>> next_states_team1(num), next_states_team2(num);
    std::vector<std::vector<bool>> dones_team1(num), dones_team2(num);
    std::vector<std::vector<int>> actions_team1(num), actions_team2(num);
    // Ensure correct structure
    SDL_assert(states.size() == rewards.size() && rewards.size() == dones.size());

    // Split data into separate vectors
    for (size_t i = 0; i < total_samples; ++i)
    {
        size_t player_index = i % num_players;
        if (player_index < num)
        {
            states_team1[player_index].push_back(states[i]);
            rewards_team1[player_index].push_back(rewards[i]);
            next_states_team1[player_index].push_back(next_states[i]);
            dones_team1[player_index].push_back(dones[i]);
            actions_team1[player_index].push_back(actions[i]);
        }
        else
        {
            auto idx = player_index - num;
            states_team2[idx].push_back(states[i]);
            rewards_team2[idx].push_back(rewards[i]);
            next_states_team2[idx].push_back(next_states[i]);
            dones_team2[idx].push_back(dones[i]);
            actions_team2[idx].push_back(actions[i]);
        }
    }
    std::vector<std::thread> threads;
    std::cout << "Training\n";
    // Launch threads for team1
    for (int i = 0; i < num; ++i)
    {
        threads.emplace_back([&, i]()
                             { team1[i].getAgent().update(states_team1[i], rewards_team1[i], next_states_team1[i], dones_team1[i], actions_team1[i]); });
    }

    // Launch threads for team2
    for (int i = 0; i < num; ++i)
    {
        threads.emplace_back([&, i]()
                             { team2[i].getAgent().update(states_team2[i], rewards_team2[i], next_states_team2[i], dones_team2[i], actions_team2[i]); });
    }
    for (auto &t : threads)
    {
        t.join();
    }

    if (current_p1->getAgent().compare(current_p2->getAgent(), states_team1[0][0]))
        std::cout << "same model\n";
    else
        std::cout << "difference model\n";
    states.clear();
    next_states.clear();
    rewards.clear();
    dones.clear();
    actions.clear();
}

void Team::save_weights()
{
    for (auto &player : team1)
    {
        player.getAgent().saveWeights("team1_player" + std::to_string(&player - &team1[0]));
    }
    for (auto &player : team2)
    {
        player.getAgent().saveWeights("team2_player" + std::to_string(&player - &team2[0]));
    }
}
bool Team::load_weights()
{
    for (auto &player : team1)
    {
        std::string filename = "team1_player" + std::to_string(&player - &team1[0]);
        std::ifstream file(filename + "_policy.bin");
        if (file.good())
        {
            player.getAgent().loadWeights(filename);
        }
        else
        {
            // SDL_assert(false && "Weights file not found for team1 player");
            return false;
        }
    }
    for (auto &player : team2)
    {
        std::string filename = "team2_player" + std::to_string(&player - &team2[0]);
        std::ifstream file(filename + "_policy.bin");
        if (file.good())
        {
            player.getAgent().loadWeights(filename);
        }
        else
        {
            // SDL_assert(false && "Weights file not found for team1 player");
            return false;
        }
    }
    return true;
}
void Team::kicking(bool team)
{
    if (team)
    {
        current_p1->set_kick();
    }
    else
    {
        current_p2->set_kick();
    }
}
void Team::reset_after_timing(bool team)
{
    if (team)
    {
        for (int i = 0; i < num; ++i)
        {
            team1[i].set_kick(true);
        }
    }
    else
    {
        for (int i = 0; i < num; ++i)
        {
            team2[i].set_kick(true);
        }
    }
}

double Team::closest(SDL_FPoint ball)
{
    double arr1[num];
    double arr2[num];
    int i = 0;
    for (auto &p : team1)
    {
        arr1[i] = distance(p.get_rect().x, p.get_rect().y, ball.x, ball.y);
        i++;
    }
    i = 0;
    for (auto &p : team2)
    {
        arr2[i] = distance(p.get_rect().x, p.get_rect().y, ball.x, ball.y);
        i++;
    }
    std::sort(&arr1[0], &arr1[num]);
    std::sort(&arr2[0], &arr2[num]);

    int count = 0;
    for (int i = 0; i < std::min(num - 1, 2); ++i)
    {
        if (arr1[i] < 7000)
            count += 2;
        if (arr2[i] < 7000)
            count -= 2;
    }
    for (int i = 0, j = 0; i + j < num;)
    {
        if (arr1[i] < arr2[j])
        {
            count++;
            i++;
        }
        else if (arr1[i] > arr2[j])
        {
            count--;
            j++;
        }
        else
        {
            i++;
            j++;
        }
    }
    // std::cout << count << std::endl;
    return count;
}