#pragma once
#include "graphics/src/text.h"
#include "graphics/src/image.h"
#include "graphics/src/menu.h"
#include "graphics/src/textbox.h"
#include "player.h"
#include "room.h"
#include "battle.h"
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <SDL.h>


enum class Mode
{
    CUTSCENE,
    NORMAL,
    BATTLE
};


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
    void add_text(SDL_Renderer* rend, SDL_Point pos, const std::string& text, const std::string& font_path, int ptsize, SDL_Color color, int delete_after_ms);
    // Lock the mutex and then emplace_back image into m_images
    void add_image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path, int delete_after_ms);
    
    // Lock the mutex and then set m_menu to menu
    void set_menu(SDL_Renderer* rend, SDL_Point pos, const std::vector<std::string>& options, int space_between_options, const std::string& font_path, int ptsize);
    // Lock mutex and delete menu
    void delete_menu();
    // Lock mutex and get the selected menu choice
    std::string get_menu_choice();
    
    // Wait for key z to be pressed
    void wait_for_z();
    
    void open_map(const std::string& map_name);
    void load_maps(const std::string& directory_name);

    void next_room();
    void prev_room();

    bool within_range(SDL_Point p1, SDL_Point p2);
    Entity* nearest_entity_in_range();

    void start_battle(Entity* ent);
    void end_battle();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_rend;

    std::string m_resources_dir;
    std::string m_font_path;

    std::atomic<bool> m_running{ true };

    std::vector<std::unique_ptr<gui::Text>> m_text;
    std::vector<std::unique_ptr<gui::Image>> m_images;
    std::unique_ptr<gui::Menu> m_menu;

    std::unique_ptr<gui::Textbox> m_dialogue_box;
    int m_dialogue_list_index{ 0 };

    std::unique_ptr<Battle> m_current_battle;

    std::mutex m_mtx;

    std::atomic<bool> m_z_down{ false };

    std::unique_ptr<Player> m_player;

    std::vector<std::unique_ptr<Room>> m_rooms;
    int m_current_room_index{ -1 };

    SDL_Texture* m_atlas;
    std::map<char, SDL_Point> m_texture_map;
    std::vector<char> m_solid_characters;

    std::map<std::string, std::vector<Entity*>> m_room_entities;

    Mode m_mode{ Mode::CUTSCENE };

    /* constants */
    const int m_player_speed{ 2 };
};
