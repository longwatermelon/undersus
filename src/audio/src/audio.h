#pragma once
#include <string>
#include <SDL_mixer.h>
#include <SDL.h>


namespace audio
{
    void play_music(const std::string& path);
    void stop_music();

    void play_sound(const std::string& path);
    void cleanup_sound_when_done();

    inline Mix_Music* music{ 0 };
    inline Mix_Chunk* sound{ 0 };
}
