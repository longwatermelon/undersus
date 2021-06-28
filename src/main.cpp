#include "game.h"
#include <thread>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>


int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);

    Game* g = new Game("res");
    std::thread thr_start(&Game::start_game, g);

    while (true)
    {
        g->mainloop();
        
        if (g->ready_to_restart())
        {
            SDL_Window* w = g->window();
            SDL_Renderer* r = g->rend();

            delete g;
            g = new Game("res", w, r);
            g->setup_game();
        }
        else
        {
            delete g;
            break;
        }
    }

    thr_start.join();

    SDL_Quit();
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
        
    return 0;
}
