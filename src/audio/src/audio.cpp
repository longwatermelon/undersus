#include "audio.h"
#include <iostream>


void audio::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        std::cout << Mix_GetError() << "\n";
    }

    Mix_ReserveChannels(8);
}


void audio::play_music(const std::string& path)
{
    audio::stop_music();

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
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());

    if (!chunk)
        std::cout << Mix_GetError() << "\n";
    else
    {
        if (Mix_PlayChannel(current_free_channel, chunk, 0) == -1)
        {
            std::cout << "error: " << Mix_GetError() << "\n";
        }
        else
        {
            sounds_playing.emplace_back(std::pair<Mix_Chunk*, int>{ chunk, current_free_channel });

            ++current_free_channel;

            if (current_free_channel >= 7)
                current_free_channel = 0;
        }
    }
}


void audio::cleanup_sound_when_done()
{
    for (int i = 0; i < sounds_playing.size(); ++i)
    {
        auto& pair = sounds_playing[i];

        if (!Mix_Playing(pair.second))
        {
            Mix_FreeChunk(pair.first);
            sounds_playing.erase(sounds_playing.begin() + i);
            --i;
        }
    }
}

