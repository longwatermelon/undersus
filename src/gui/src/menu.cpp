#include "menu.h"


gui::Menu::Menu(SDL_Renderer* rend, SDL_Point pos, const std::vector<std::string>& options, int space_between_options, const std::string& font_path, int ptsize)
    : m_pos(pos), m_space_between_options(space_between_options)
{
    int x = m_pos.x;

    for (auto& opt : options)
    {
        m_options.emplace_back(new gui::Text(rend, { x, m_pos.y }, opt, font_path, ptsize, { 255, 255, 255 }, -1));
        x += space_between_options;
    }

    move_selected(0);
}


gui::Menu::~Menu()
{
    m_options.clear();
}


void gui::Menu::render()
{
    for (auto& opt : m_options)
    {
        opt->render();
    }
}


void gui::Menu::move_selected(int x)
{
    m_options[m_selected_index]->change_color({ 255, 255, 255 });
    m_selected_index = std::min(std::max(m_selected_index + x, 0), (int)m_options.size() - 1);
    m_options[m_selected_index]->change_color({ 255, 255, 0 });
}

