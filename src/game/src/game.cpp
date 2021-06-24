#include "game.h"
#include <thread>
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
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                m_running = false;
                break;
            }
        }

        SDL_RenderClear(m_rend);
        
        {
            std::lock_guard lock(m_mtx);

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
        }        
        

        SDL_RenderPresent(m_rend);
    }

    m_text.clear();
    m_images.clear();
}


void Game::start_game()
{
    sleep(1000);
    add_image(new gui::Image(m_rend, { 0, 0 }, m_resources_dir + "gfx/logo.png", 5000));
    sleep(5000);
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

