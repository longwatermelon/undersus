#include "game.h"
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
    
    m_text.emplace_back(new gui::Text(m_rend, { 100, 100 }, "text", m_font_path, 16, { 255, 255, 255 }, 1000));
    m_images.emplace_back(new gui::Image(m_rend, { 100, 200 }, m_resources_dir + "gfx/image.png", 800));

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

        SDL_RenderPresent(m_rend);
    }

    m_text.clear();
}

