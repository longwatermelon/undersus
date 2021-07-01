#pragma once
#include <string>
#include <vector>
#include <SDL_mixer.h>
#include <SDL.h>


namespace audio
{
    void init();

    void play_music(const std::string& path);
    void stop_music();

    void play_sound(const std::string& path);
    void cleanup_sound_when_done();

    inline Mix_Music* music{ 0 };

    inline int current_free_channel{ 0 };
    inline std::vector<std::pair<Mix_Chunk*, int>> sounds_playing;
}
