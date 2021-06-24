#include "game.h"
#include <thread>


int main()
{
    Game g("res");
    std::thread thr_render(&Game::mainloop, &g);
    
    g.start_game();

    thr_render.join();

    return 0;
}
