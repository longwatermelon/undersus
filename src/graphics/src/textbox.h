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
        Textbox(SDL_Renderer* rend, SDL_Rect rect, const std::string& text, const std::string& font_path, int ptsize, bool draw_outline, SDL_Color m_bg_color, SDL_Color font_color);
        ~Textbox();

        void render();

        void add_char();

        std::string next_word();

    private:
        SDL_Rect m_rect;
        SDL_Rect m_background_rect;

        SDL_Renderer* m_rend;
        std::string m_text;

        std::vector<std::string> m_displayed_text{ "" };

        std::vector<SDL_Texture*> m_textures{ 0 };
        common::Font m_font;

        std::chrono::system_clock::time_point m_last_added_char_time;

        SDL_Point m_char_dim;

        bool m_draw_outline{ false };
        SDL_Color m_bg_color;
        SDL_Color m_font_color;
    };
}

