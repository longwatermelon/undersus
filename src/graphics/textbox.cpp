#include "textbox.h"
#include <iostream>


gui::Textbox::Textbox(SDL_Renderer* rend, const std::string& text, const std::string& font_path, int ptsize)
    : m_rend(rend), m_text(text)
{
    m_font = { TTF_OpenFont(font_path.c_str(), ptsize), ptsize };
    m_rect = { 25, 20, 0, 0 };
    m_last_added_char_time = std::chrono::system_clock::now();
    add_char();

    TTF_SizeText(m_font.font, " ", &m_char_dim.x, &m_char_dim.y);
}


gui::Textbox::~Textbox()
{
    for (auto& tex : m_textures)
    {
        SDL_DestroyTexture(tex);
    }
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

    for (int i = 0; i < m_textures.size(); ++i)
    {
        if (!m_textures[i])
            continue;

        tmp = m_rect;
        tmp.y = 20 + i * m_char_dim.y;
        TTF_SizeText(m_font.font, m_displayed_text[i].c_str(), &tmp.w, &tmp.h);

        SDL_RenderCopy(m_rend, m_textures[i], 0, &tmp);
    }
}


void gui::Textbox::add_char()
{
    int total_length = 0;

    for (auto& str : m_displayed_text)
    {
        total_length += str.size();
    }

    if (total_length >= m_text.size())
        return;

    std::string current_string = m_displayed_text[m_displayed_text.size() - 1];

    int width;
    TTF_SizeText(m_font.font, (m_displayed_text[m_displayed_text.size() - 1] + next_word()).c_str(), &width, 0);

    if (width >= m_background_rect.w && current_string[current_string.size() - 1] == ' ')
    {
        m_displayed_text.emplace_back("");
        m_textures.emplace_back(nullptr);
    }

    m_displayed_text[m_displayed_text.size() - 1] += m_text[total_length];

    // reference to pointer since tex will change when assigned to a new value
    SDL_Texture*& tex = m_textures[m_textures.size() - 1];

    if (tex)
        SDL_DestroyTexture(tex);

    tex = common::render_text(m_rend, m_font, m_displayed_text[m_displayed_text.size() - 1], { 255, 255, 255 });
    TTF_SizeText(m_font.font, m_displayed_text[m_displayed_text.size() - 1].c_str(), &m_rect.w, &m_rect.h);
}


std::string gui::Textbox::next_word()
{
    int index = m_displayed_text[m_displayed_text.size() - 1].size() - 1;

    while (index < m_text.size() && m_text[index++] != ' ');

    std::string word;

    while (index < m_text.size() && m_text[index] != ' ')
    {
        word += m_text[index++];
    }

    return word;
}

