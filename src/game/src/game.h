#pragma once
#include "text.h"
#include "image.h"
#include "menu.h"
#include "player.h"
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

    // Event handling and rendering
    void mainloop();

    // See function body
    void start_game();

private:
    // std::this_thread::sleep_for except it kills the thread when m_running is set to false
    void sleep(int ms);
   
    // Lock the mutex and then emplace_back text into m_text 
    void add_text(gui::Text* text);
    // Lock the mutex and then emplace_back image into m_images
    void add_image(gui::Image* image);
    
    // Lock the mutex and then set m_menu to menu
    void set_menu(gui::Menu* menu);
    // Lock mutex and delete menu
    void delete_menu();
    // Lock mutex and get the selected menu choice
    std::string get_menu_choice();
    
    // Wait for key z to be pressed
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

    std::unique_ptr<Player> m_player;

    /* constants */
    const int m_player_speed{ 3 };
};
