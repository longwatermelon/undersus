#pragma once
#include <string>
#include <chrono>
#include <SDL.h>


namespace gui
{
    class Image
    {
    public:
        Image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path, int delete_after_ms);
        ~Image();

        void render();

        bool overtime() { return m_overtime; }

    private:
        SDL_Rect m_rect;

        SDL_Texture* m_tex;
        SDL_Renderer* m_rend;

        int m_delete_after_ms{ -1 };
        bool m_overtime{ false };

        std::chrono::system_clock::time_point m_creation_time;
    };
}
