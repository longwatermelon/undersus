#pragma once
#include <string>
#include <SDL.h>


namespace gui
{
    class Image
    {
    public:
        Image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path);

        void render();

    private:
        SDL_Rect m_rect;

        SDL_Texture* m_tex;
        SDL_Renderer* m_rend;
    };
}
