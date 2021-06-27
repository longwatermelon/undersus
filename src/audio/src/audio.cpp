#include "audio.h"
#include <iostream>


void audio::play_music(const std::string& path)
{
    audio::stop_music();

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
    if (music)
    {
        Mix_FreeMusic(music);
        music = 0;
    }
}


void audio::play_sound(const std::string& path)
{
    if (sound)
    {
        Mix_FreeChunk(sound);
        sound = 0;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << Mix_GetError() << "\n";
    }

    sound = Mix_LoadWAV(path.c_str());
    
    if (!sound)
        std::cout << Mix_GetError() << "\n";
    else
    {
        Mix_PlayChannel(-1, sound, 0);
    }
}


void audio::cleanup_sound_when_done()
{
    if (sound && !Mix_Playing(-1))
    {
        Mix_FreeChunk(sound);
        sound = 0;
    }
}

