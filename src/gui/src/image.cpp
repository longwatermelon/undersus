#include "image.h"
#include <SDL_image.h>


gui::Image::Image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path, int delete_after_ms)
    : m_rend(rend), m_rect{ pos.x, pos.y }, m_delete_after_ms(delete_after_ms)
{
    m_tex = IMG_LoadTexture(rend, image_path.c_str());
    SDL_QueryTexture(m_tex, 0, 0, &m_rect.w, &m_rect.h);
    
    m_creation_time = std::chrono::system_clock::now();
}


gui::Image::~Image()
{
    SDL_DestroyTexture(m_tex);
}


void gui::Image::render()
{
    SDL_RenderCopy(m_rend, m_tex, 0, &m_rect);

    if (m_delete_after_ms >= 0 && std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_creation_time).count() >= m_delete_after_ms)
    {
        m_overtime = true;
    }
}

