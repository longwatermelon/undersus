#pragma once
#include "common.h"
#include <string>
#include <chrono>
#include <SDL.h>


namespace gui
{
    class Textbox
    {
    public:
        Textbox(SDL_Renderer* rend, const std::string& text, const std::string& font_path, int ptsize);

        void render();

        void add_char();

    private:
        SDL_Rect m_rect;
        SDL_Rect m_background_rect{ 20, 20, 800 - 40, 60 };

        SDL_Renderer* m_rend;
        std::string m_text;
        std::string m_displayed_text;

        SDL_Texture* m_tex{ 0 };
        common::Font m_font;

        std::chrono::system_clock::time_point m_last_added_char_time;
    };
}

