#pragma once
#include "text.h"
#include <vector>
#include <string>
#include <memory>
#include <SDL.h>


namespace gui
{
    class Menu
    {
    public:
        Menu(SDL_Renderer* rend, SDL_Point pos, const std::vector<std::string>& options, int space_between_options, const std::string& font_path, int ptsize);
        ~Menu();

        void render();

        void move_selected(int x);

        std::string selected_opt() { return m_options[m_selected_index]->text(); }

    private:
        SDL_Point m_pos;

        std::vector<std::unique_ptr<Text>> m_options;
        int m_space_between_options;

        int m_selected_index{ 0 };
    };
}
