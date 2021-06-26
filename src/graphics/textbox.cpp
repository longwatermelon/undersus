#include "textbox.h"


gui::Textbox::Textbox(SDL_Renderer* rend, const std::string& text, const std::string& font_path, int ptsize)
    : m_rend(rend), m_text(text)
{
    m_font = { TTF_OpenFont(font_path.c_str(), ptsize), ptsize };
    m_rect = { 25, 20, 0, 0 };
    m_last_added_char_time = std::chrono::system_clock::now();
}


void gui::Textbox::render()
{
    if (std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_last_added_char_time).count() >= 25)
    {
        add_char();
        m_last_added_char_time = std::chrono::system_clock::now();
    }

    SDL_Rect tmp = {
        m_background_rect.x - 5,
        m_background_rect.y - 5,
        m_background_rect.w + 10,
        m_background_rect.h + 10
    };

    SDL_SetRenderDrawColor(m_rend, 255, 255, 255, 255);
    SDL_RenderFillRect(m_rend, &tmp);

    SDL_SetRenderDrawColor(m_rend, 0, 0, 0, 255);
    SDL_RenderFillRect(m_rend, &m_background_rect);

    if (m_tex)
        SDL_RenderCopy(m_rend, m_tex, 0, &m_rect);
}


void gui::Textbox::add_char()
{
    if (m_displayed_text.size() == m_text.size())
        return;

    m_displayed_text += m_text[m_displayed_text.size()];

    if (m_tex)
        SDL_DestroyTexture(m_tex);

    m_tex = common::render_text(m_rend, m_font, m_displayed_text, { 255, 255, 255 });
    TTF_SizeText(m_font.font, m_displayed_text.c_str(), &m_rect.w, &m_rect.h);
}

