#pragma once
#include <string>
#include <SDL_mixer.h>
#include <SDL.h>


namespace audio
{
    void play_sound(const std::string& path);
    void stop_music();

    inline Mix_Music* music{ 0 };
}
