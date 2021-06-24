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

        bool overtime() { return m_overtime; }

    private:
        SDL_Rect m_rect;
        std::string m_text;

        SDL_Renderer* m_rend;
        SDL_Texture* m_tex;

        TTF_Font* m_font;

        int m_delete_after_ms{ -1 };
        bool m_overtime{ false };

        std::chrono::system_clock::time_point m_creation_time;
    };
}
