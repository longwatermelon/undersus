#pragma once
#include "text.h"
#include "image.h"
#include "menu.h"
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <SDL.h>


class Game
{
public:
    Game(const std::string& resources_path);
    ~Game();

    void mainloop();

    void start_game();

private:
    void sleep(int ms);
    
    void add_text(gui::Text* text);
    void add_image(gui::Image* image);

    void set_menu(gui::Menu* menu);
    void delete_menu();
    std::string get_menu_choice();

    void wait_for_z();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_rend;

    std::string m_resources_dir;
    std::string m_font_path;

    std::atomic<bool> m_running{ true };

    std::vector<std::unique_ptr<gui::Text>> m_text;
    std::vector<std::unique_ptr<gui::Image>> m_images;
    std::unique_ptr<gui::Menu> m_menu;

    std::mutex m_mtx;

    std::atomic<bool> m_z_down{ false };
};
