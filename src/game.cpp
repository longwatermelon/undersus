#include "game.h"
#include "menu.h"
#include "room.h"
#include "audio/audio.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL_image.h>
#include <SDL_mixer.h>


Game::Game(const std::string& resources_path)
    : m_resources_dir(resources_path + '/')
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);

    m_window = SDL_CreateWindow("Undersus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderClear(m_rend);
    SDL_RenderPresent(m_rend);

    m_font_path = m_resources_dir + "gfx/font.ttf";
    m_atlas = IMG_LoadTexture(m_rend, (m_resources_dir + "gfx/atlas.png").c_str());

    m_texture_map['#'] = { 0, 0 };
    m_texture_map['.'] = { 32, 0 };
    m_solid_characters = { '#' };
}


Game::~Game()
{
    SDL_DestroyTexture(m_atlas);

    SDL_DestroyRenderer(m_rend);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
}


void Game::mainloop()
{
    std::thread thr_start(&Game::start_game, this);

    SDL_Event evt;
    
    while (m_running)
    {
        {
            std::lock_guard lock(m_mtx);

            while (SDL_PollEvent(&evt))
            {
                switch (evt.type)
                {
                case SDL_QUIT:
                    m_running = false;
                    goto cleanup;
                    break;
                case SDL_KEYDOWN:
                {
                    switch (evt.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                        if (m_menu.get())
                            m_menu->move_selected(1);

                        if (m_player)
                            m_player->set_x_vel(m_player_speed);

                        break;
                    case SDLK_LEFT:
                        if (m_menu.get())
                            m_menu->move_selected(-1);

                        if (m_player)
                            m_player->set_x_vel(-m_player_speed);

                        break;
                    case SDLK_UP:
                        if (m_player)
                            m_player->set_y_vel(-m_player_speed);

                        break;
                    case SDLK_DOWN:
                        if (m_player)
                            m_player->set_y_vel(m_player_speed);

                        break;
                    case SDLK_z:
                        m_z_down = true;
                        break;
                    }
                } break;
                case SDL_KEYUP:
                {
                    switch (evt.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                    case SDLK_LEFT:
                        if (m_player)
                            m_player->set_x_vel(0);

                        break;
                    case SDLK_UP:
                    case SDLK_DOWN:
                        if (m_player)
                            m_player->set_y_vel(0);

                        break;
                    case SDLK_z:
                        m_z_down = false;
                        break;
                    }
                } break;
                }
            }

            SDL_RenderClear(m_rend);

            if (m_current_room_index != -1 && m_current_room_index < m_rooms.size())
                m_rooms[m_current_room_index]->render();

            for (int i = 0; i < m_text.size(); ++i)
            {
                m_text[i]->render();

                if (m_text[i]->overtime())
                {
                    m_text.erase(m_text.begin() + i);
                    --i;
                }
            }

            for (int i = 0; i < m_images.size(); ++i)
            {
                m_images[i]->render();

                if (m_images[i]->overtime())
                {
                    m_images.erase(m_images.begin() + i);
                    --i;
                }
            }

            if (m_player)
            {
                m_player->move(m_rooms[m_current_room_index]->layout(), m_rooms[m_current_room_index]->characters_per_line(), m_solid_characters);
                
                if (m_player->rect().x + m_player->rect().w >= 790)
                    next_room();

                if (m_player->rect().x <= 10)
                    prev_room();

                m_player->render();
            }

            if (m_menu)
                m_menu->render();

            SDL_RenderPresent(m_rend);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

cleanup: 
    if (thr_start.joinable())
        thr_start.join();

    m_text.clear();
    m_images.clear();

    delete_menu();

    m_rooms.clear();
}


void Game::start_game()
{
    sleep(1000);
    
    add_image(m_rend, { 0, 0 }, m_resources_dir + "gfx/logo.png", 4000);

    sleep(5000);
    
    set_menu(m_rend, { 200, 100 }, { "Start" }, 100, m_font_path, 16);

    wait_for_z();
    
    delete_menu();
    
    {
        std::lock_guard lock(m_mtx);
        m_player = std::unique_ptr<Player>(new Player(m_rend, { 200, 200, 32, 32 }, m_resources_dir + "gfx/player.png"));
    }

    open_map("start");
    open_map("start_2");

    {
        std::lock_guard lock(m_mtx);
        next_room();
    }

    audio::play_sound(m_resources_dir + "sfx/among_us_drip.wav");
}


void Game::sleep(int ms)
{
    if (!m_running)
        return; 

    auto start = std::chrono::system_clock::now();

    while (m_running && std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - start).count() <= ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}


void Game::add_text(SDL_Renderer* rend, SDL_Point pos, const std::string& text, const std::string& font_path, int ptsize, SDL_Color color, int delete_after_ms)
{
    if (!m_running)
        return;

    std::lock_guard lock(m_mtx);
    m_text.emplace_back(new gui::Text(rend, pos, text, font_path, ptsize, color, delete_after_ms));
}


void Game::add_image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path, int delete_after_ms)
{
    if (!m_running)
        return;

    std::lock_guard lock(m_mtx);
    m_images.emplace_back(new gui::Image(rend, pos, image_path, delete_after_ms));
}


void Game::wait_for_z()
{
    while (m_running && !m_z_down)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    if (!m_running)
        return;

    m_z_down = false;
}


void Game::set_menu(SDL_Renderer* rend, SDL_Point pos, const std::vector<std::string>& options, int space_between_options, const std::string& font_path, int ptsize)
{
    std::lock_guard lock(m_mtx);
    m_menu = std::unique_ptr<gui::Menu>(new gui::Menu(rend, pos, options, space_between_options, font_path, ptsize));
}


void Game::delete_menu()
{
    std::lock_guard lock(m_mtx);
    m_menu.reset(0);
}


std::string Game::get_menu_choice()
{
    std::lock_guard lock(m_mtx);
    return m_menu->selected_opt();
}


void Game::open_map(const std::string& map_name)
{
    std::ifstream ifs(m_resources_dir + "maps/" + map_name + ".txt");
    std::stringstream ss;
    std::string buf;

    SDL_Point lpos, rpos;

    std::getline(ifs, buf);
    lpos.x = std::stoi(buf) * 32;

    std::getline(ifs, buf);
    lpos.y = std::stoi(buf) * 32;

    std::getline(ifs, buf);
    rpos.x = std::stoi(buf) * 32;

    std::getline(ifs, buf);
    rpos.y = std::stoi(buf) * 32;
    
    while (std::getline(ifs, buf)) ss << buf;

    ifs.close();
    
    {
        std::lock_guard lock(m_mtx);
        m_rooms.emplace_back(new Room(m_rend, ss.str(), 25, m_texture_map, m_atlas, lpos, rpos));
    } 
}


void Game::next_room()
{
    ++m_current_room_index;
    m_player->move_to(m_rooms[m_current_room_index]->left_start_pos().x, m_rooms[m_current_room_index]->left_start_pos().y);
}


void Game::prev_room()
{
    --m_current_room_index;
    m_player->move_to(m_rooms[m_current_room_index]->right_start_pos().x, m_rooms[m_current_room_index]->right_start_pos().y);
}

