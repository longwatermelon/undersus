#include "game.h"


Game::Game(const std::string& resources_path)
    : m_resources_dir(resources_path)
{
    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow("Undersus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderClear(m_rend);
    SDL_RenderPresent(m_rend);
}


Game::~Game()
{
    SDL_DestroyRenderer(m_rend);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
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


        SDL_RenderPresent(m_rend);
    }
}

