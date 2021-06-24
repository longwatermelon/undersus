#pragma once
#include "common.h"
#include <string>
#include <SDL.h>


namespace gui
{
    class Text
    {
    public:
        Text(SDL_Renderer* rend, SDL_Point pos, const std::string& text, const std::string& font_path, int ptsize, SDL_Color color);
        ~Text();

        void render();

    private:
        SDL_Rect m_rect;
        std::string m_text;

        SDL_Renderer* m_rend;
        SDL_Texture* m_tex;

        TTF_Font* m_font;
    };
}
