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
}


void gui::Menu::render()
{
    for (auto& opt : m_options)
    {
        opt->render();
    }
}

