#include "game.h"
#include "audio/audio.h"
#include <thread>
#include <iostream>


int main()
{
    Game g("res");
    g.mainloop();
    
    return 0;
}
