#include "text.h"


gui::Text::Text(SDL_Renderer* rend, SDL_Point pos, const std::string& text, const std::string& font_path, int ptsize, SDL_Color color, int delete_after_ms)
    : m_rend(rend), m_rect{ pos.x, pos.y }, m_text(text), m_delete_after_ms(delete_after_ms), m_color(color), m_ptsize(ptsize)
{
    m_font = TTF_OpenFont(font_path.c_str(), ptsize);
    m_tex = common::render_text(m_rend, { m_font, ptsize }, text, color);
    
    TTF_SizeText(m_font, text.c_str(), &m_rect.w, &m_rect.h);

    m_creation_time = std::chrono::system_clock::now();
}


gui::Text::~Text()
{
    SDL_DestroyTexture(m_tex);
    TTF_CloseFont(m_font);
}


void gui::Text::render()
{
    SDL_RenderCopy(m_rend, m_tex, 0, &m_rect);

    if (m_delete_after_ms >= 0 && std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_creation_time).count() >= m_delete_after_ms)
    {
        m_overtime = true;
    }
}


void gui::Text::change_color(SDL_Color color)
{
    m_color = color;
    m_tex = common::render_text(m_rend, { m_font, m_ptsize }, m_text, m_color);
}

