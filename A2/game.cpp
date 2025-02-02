#include "game.hpp"
bool Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "SDL couldn't initialize, SDL_error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    if (TTF_Init() == -1) {
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

    txtr_manager.load_text_texture(TextureType::HOME_MESSAGE, "HOME", config::FONT_PATH, config::HomeColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::AWAY_MESSAGE, "AWAY", config::FONT_PATH, config::AwayColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::HOME_SCORE, "0", config::FONT_PATH, config::HomeColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::AWAY_SCORE, "0", config::FONT_PATH, config::AwayColor, config::FONT_SIZE, m_renderer);
    txtr_manager.load_text_texture(TextureType::GOAL_MESSAGE, "GOAL!!!", config::FONT_PATH, config::WHITE, 200, m_renderer);
    txtr_manager.load_text_texture(TextureType::GOAL_RATIO, "0 : 0", config::FONT_PATH, config::WHITE, 200, m_renderer);
    bg = Background(&txtr_manager, m_window, m_renderer);
    bg.draw_score();
    PlayerFactory player_factory = PlayerFactory();

    teams.new_team(player_factory.create(2, true));
    teams.new_team(player_factory.create(2, false));
    ball = Ball(&txtr_manager, m_renderer);
}

int Game::run()
{
    bool done = false;
    SDL_Event event;
    uint32_t last_frame_time = 0, current_frame_time = 0, delta_time = 0, switch_delay = 100;
    uint32_t last_action_1 = 0, last_action_2 = 0, last_switch_1 = 0, last_switch_2 = 0;
    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
            if (SDL_QUIT == event.type)
            {
                done = true;
                break;
            }
            else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_TAB:
                    teams.switch_player(true, ball.get_pos());
                    break;
                case SDL_SCANCODE_BACKSLASH:
                    std::cout<<"backslash\n";
                    teams.switch_player(false, ball.get_pos());
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
        teams.move();
        ball.move(config::BORDER, teams.get_rect_list());
        auto result = ball.scoring();
        if (result == 1) {
            bg.update_score_home();
        }
        else if (result == 0) {
            bg.update_score_away();
        }
        std::cout << "=================================\n";
        if (delta_time > 1000 / config::FPS)
        {
            last_frame_time = current_frame_time;
            this->draw();
        }
    }
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void Game::draw()
{
    // std::cout << "Game renderer: " << &m_renderer << std::endl;
    // SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    bg.draw();
    ball.draw();
    teams.draw(m_renderer);

    // SDL_SetRenderTarget(m_renderer, NULL);
    SDL_RenderPresent(m_renderer);
}

Game::~Game() {}