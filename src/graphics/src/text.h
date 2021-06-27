#pragma once
#include "common.h"
#include <string>
#include <chrono>
#include <SDL.h>


namespace gui
{
    class Text
    {
    public:
        Text(SDL_Renderer* rend, SDL_Point pos, const std::string& text, const std::string& font_path, int ptsize, SDL_Color color, int delete_after_ms);
        ~Text();

        void render();

        void change_color(SDL_Color color);

        bool overtime() { return m_overtime; }
        std::string text() { return m_text; }

    private:
        SDL_Rect m_rect;
        std::string m_text;

        SDL_Renderer* m_rend;
        SDL_Texture* m_tex;

        TTF_Font* m_font;
        int m_ptsize;

        int m_delete_after_ms{ -1 };
        bool m_overtime{ false };

        std::chrono::system_clock::time_point m_creation_time;

        SDL_Color m_color;
    };
}
