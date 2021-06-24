#include "game.h"
#include "menu.h"
#include <thread>
#include <iostream>
#include <SDL_image.h>


Game::Game(const std::string& resources_path)
    : m_resources_dir(resources_path + '/')
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    m_window = SDL_CreateWindow("Undersus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderClear(m_rend);
    SDL_RenderPresent(m_rend);

    m_font_path = m_resources_dir + "gfx/font.ttf";
}


Game::~Game()
{
    SDL_DestroyRenderer(m_rend);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
}


void Game::mainloop()
{
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
                    break;
                case SDL_KEYDOWN:
                {
                    switch (evt.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                        if (m_menu.get())
                            m_menu->move_selected(1);
                        break;
                    case SDLK_LEFT:
                        if (m_menu.get())
                            m_menu->move_selected(-1);
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
                    case SDLK_z:
                        m_z_down = false;
                        break;
                    }
                } break;
                }
            }

            SDL_RenderClear(m_rend);

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

            if (m_menu)
                m_menu->render();

            SDL_RenderPresent(m_rend);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    m_text.clear();
    m_images.clear();

    delete_menu();
}


void Game::start_game()
{
    sleep(1000);
    
    set_menu(new gui::Menu(m_rend, { 100, 100 }, { "text", "text 2" }, 100, m_font_path, 16));

    wait_for_z();
    
    std::cout << get_menu_choice() << "\n";
    delete_menu();

    m_running = false;
}


void Game::sleep(int ms)
{
    if (!m_running)
        exit(0);

    auto start = std::chrono::system_clock::now();

    while (m_running && std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - start).count() <= ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}


void Game::add_text(gui::Text* text)
{
    if (!m_running)
        return;

    std::lock_guard lock(m_mtx);
    m_text.emplace_back(text);
}


void Game::add_image(gui::Image* image)
{
    if (!m_running)
        return;

    std::lock_guard lock(m_mtx);
    m_images.emplace_back(image);
}


void Game::wait_for_z()
{
    while (m_running && !m_z_down)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    if (!m_running)
        exit(0);

    m_z_down = false;
}


void Game::set_menu(gui::Menu* menu)
{
    std::lock_guard lock(m_mtx);
    m_menu = std::unique_ptr<gui::Menu>(menu);
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

