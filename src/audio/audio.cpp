#include "audio.h"
#include <iostream>


void audio::play_sound(const std::string& path)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << Mix_GetError() << "\n";
    }

    music = Mix_LoadMUS(path.c_str());

    if (!music)
        std::cout << Mix_GetError() << "\n";
    else
    {
        Mix_PlayMusic(music, -1);
    }
}


void audio::stop_music()
{
    Mix_FreeMusic(music);
    music = 0;
}

