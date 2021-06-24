#include "game.h"
#include <thread>


int main()
{
    Game g("res");
    std::thread thr_start(&Game::start_game, &g);

    g.mainloop();

    thr_start.join();

    return 0;
}
