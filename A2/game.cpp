#include "game.hpp"
bool Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "SDL couldn't initialize, SDL_error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    if (TTF_Init() == -1)
    {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << "\n";
        return EXIT_FAILURE;
    }

    m_window = SDL_CreateWindow(
        "Hello World",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        config::SCREEN_WIDTH, config::SCREEN_HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI);

    if (!m_window)
    {
        std::cerr << "Cant create window, SDL_error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    // m_surface = SDL_GetWindowSurface(m_window);
    return EXIT_SUCCESS;
}

Game::Game()
{
    if (init() == EXIT_FAILURE)
    {
        throw std::runtime_error("Failed to create object");
    }
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer)
    {
        std::cerr << SDL_GetError() << std::endl;
        throw std::runtime_error("Failed to create renderer");
    }
    txtr_manager.load_texture(TextureType::HOME_PLAYER, config::PLAYERSPRITE_PATH, m_renderer);
    txtr_manager.load_texture(TextureType::BALL_LEFT1, config::BALLSPRITE_PATH, m_renderer);
    txtr_manager.load_text_texture(TextureType::HOME_MESSAGE, "HOME", config::FONT_PATH, config::HomeColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::AWAY_MESSAGE, "AWAY", config::FONT_PATH, config::AwayColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::HOME_SCORE, "0", config::FONT_PATH, config::HomeColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::AWAY_SCORE, "0", config::FONT_PATH, config::AwayColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::GOAL_MESSAGE, "GOAL!!!", config::FONT_PATH, config::WHITE, 200, m_renderer);
    txtr_manager.load_text_texture(TextureType::GOAL_RATIO, "0 : 0", config::FONT_PATH, config::WHITE, 200, m_renderer);
    txtr_manager.load_text_texture(TextureType::TIMER, "00:00", config::FONT_PATH, config::WHITE, config::FONT_SIZE, m_renderer);
    bg = Background(&txtr_manager, m_window, m_renderer);
    bg.draw_score();
    PlayerFactory player_factory = PlayerFactory();
    teams = Team(&txtr_manager);
    teams.new_team(player_factory.create(config::NUM_PLAYERS_PER_TEAM, true));
    teams.new_team(player_factory.create(config::NUM_PLAYERS_PER_TEAM, false));
    ball = Ball(&txtr_manager, m_renderer);
    teams.load_weights();
}

int Game::run()
{
    bool done = false;
    bool drawing = true;
    SDL_Event event;
    uint32_t last_frame_time = 0, current_frame_time = 0, delta_time = 0, switch_delay = 100;
    uint32_t last_action_1 = 0, last_action_2 = 0, last_switch_1 = 0, last_switch_2 = 0, kick_timing_1 = 0, kick_timing_2 = 0;
    uint64_t game_timer = 60 * 1000;
    while (!done)
    {
        if (SDL_GetTicks64() > game_timer)
        {
            // SDL_Delay(1000);
            bg.reset_layout();
            teams.train();
            teams.init_pos(rand() % 2 == 0);
            game_timer = SDL_GetTicks64() + 61 * 1000; // Reset the timer after training
        }
        while (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            {
                done = true;
                break;
            }
            else if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_TAB:
                    teams.switch_player(true, ball.get_pos());
                    break;
                case SDL_SCANCODE_P:
                    std::cout << "swap\n";
                    teams.switch_player(false, ball.get_pos());
                    break;
                case SDL_SCANCODE_SPACE:
                    if (SDL_GetTicks() - kick_timing_1 > 100)
                    {
                        std::cout << "kick\n";
                        teams.kicking(true);
                        kick_timing_1 = SDL_GetTicks();
                    }
                    break;
                case SDL_SCANCODE_O:
                    if (SDL_GetTicks() - kick_timing_2 > 100)
                    {
                        std::cout << "kick\n";
                        teams.kicking(false);
                        kick_timing_2 = SDL_GetTicks();
                    }
                    break;
                default:
                    break;
                }
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                {
                    done = true;
                    break;
                }
                case SDL_SCANCODE_R:
                {
                    if (done == true)
                    {
                        done = false;
                        bg.reset_layout();
                        teams.init_pos(true);
                    }
                }
                case SDL_SCANCODE_6:
                {
                    drawing = !drawing;
                }
                // case SDL_SCANCODE_W:
                //     teams.move_up(true);
                //     break;
                // case SDL_SCANCODE_S:
                //     teams.move_down(true);
                //     break;

                // case SDL_SCANCODE_LEFT:
                //     teams.move_left(false);
                //     break;
                // case SDL_SCANCODE_RIGHT:
                //     teams.move_right(false);
                //     break;
                // case SDL_SCANCODE_UP:
                //     teams.move_up(false);
                //     break;
                // case SDL_SCANCODE_DOWN:
                //     teams.move_down(false);
                //     break;
                default:
                    break;
                }
            }
        }
        auto keys = SDL_GetKeyboardState(NULL);
        if (SDL_GetTicks() - last_action_1 > config::ACTION_TIME_LIMIT)
        {
            if (keys[SDL_SCANCODE_A])
            {
                teams.move_left(true);
            }
            if (keys[SDL_SCANCODE_D])
            {
                teams.move_right(true);
            }
            if (keys[SDL_SCANCODE_W])
            {
                teams.move_up(true);
            }
            if (keys[SDL_SCANCODE_S])
            {
                teams.move_down(true);
            }
            last_action_1 = SDL_GetTicks();
        }
        if (SDL_GetTicks() - last_action_2 > config::ACTION_TIME_LIMIT)
        {
            if (keys[SDL_SCANCODE_LEFT])
            {
                teams.move_left(false);
            }
            if (keys[SDL_SCANCODE_RIGHT])
            {
                teams.move_right(false);
            }
            if (keys[SDL_SCANCODE_UP])
            {
                teams.move_up(false);
            }
            if (keys[SDL_SCANCODE_DOWN])
            {
                teams.move_down(false);
            }
            last_action_2 = SDL_GetTicks();
        }

        current_frame_time = SDL_GetTicks();
        delta_time = current_frame_time - last_frame_time;
        if (delta_time != 0)
            if (delta_time > 1000 / config::FPS)
            {
                last_frame_time = current_frame_time;
                teams.move(ball.get_pos());
                ball.move(config::BORDER, teams.get_rect_list());
                printf("FPS: %d\n", 1000 / delta_time);
                auto result = ball.scoring();
                if (result != -1)
                {
                    if (result == 1)
                    {
                        teams.get_reward(1000.0);
                        bg.update_score_home();
                        teams.init_pos(true);
                    }
                    else if (result == 0)
                    {
                        teams.get_reward(-1000.0);
                        bg.update_score_away();
                        teams.init_pos(false);
                    }
                }
                else
                {
                    // std::cout << ball.get_pos().x << " " <<ball.get_pos().y << "\n";
                    // std::cout << ball.get_pos().x - ((config::GOAL_RIGHT_X - config::GOAL_LEFT_X) / 2 + config::BORDER.x) << "\n";
                    // std::cout << abs(ball.get_pos().y - (config::GOAL_BOTTOM_Y - config::GOAL_TOP_Y) / 2 - config::GOAL_TOP_Y) << "\n";
                    double reward = ((ball.get_pos().x - ((config::GOAL_RIGHT_X - config::GOAL_LEFT_X) / 2 + config::BORDER.x)) + // 600 max
                                     (400 - (abs(ball.get_pos().y - (config::GOAL_BOTTOM_Y - config::GOAL_TOP_Y) / 2 - config::GOAL_TOP_Y)))) *
                                    (1.0 / 950);

                    reward += teams.closest(ball.get_pos()) / 20.0;

                    std::cout << "reward = " << reward << std::endl;
                    teams.get_reward(reward);
                }
                std::cout << "=================================\n";
                if (current_frame_time - kick_timing_1 > 500)
                {
                    teams.reset_after_timing(true);
                }
                if (current_frame_time - kick_timing_2 > 500)
                {
                    teams.reset_after_timing(false);
                }
                teams.train();
            }
        this->draw(last_frame_time, drawing);
    }
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    teams.save_weights();
    return EXIT_SUCCESS;
}

void Game::draw(uint32_t last_time_frame, bool drawing)
{
    if (!drawing)
        return;
    static uint32_t last_time = 0;
    uint32_t current_time = SDL_GetTicks();
    bg.draw(current_time);
    teams.draw(m_renderer);
    if (last_time_frame - last_time > 1000 / 30)
    {
        ball.draw(true);
        last_time = last_time_frame;
    }
    else
    {
        ball.draw(false);
    }

    SDL_RenderPresent(m_renderer);
}

Game::~Game() {}