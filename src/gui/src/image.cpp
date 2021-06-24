#include "image.h"
#include <SDL_image.h>


gui::Image::Image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path)
    : m_rend(rend), m_rect{ pos.x, pos.y }
{
    m_tex = IMG_LoadTexture(rend, image_path.c_str());
    SDL_QueryTexture(m_tex, 0, 0, &m_rect.w, &m_rect.h);
}


void gui::Image::render()
{
    SDL_RenderCopy(m_rend, m_tex, 0, &m_rect);
}

