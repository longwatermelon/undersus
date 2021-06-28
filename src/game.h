#pragma once
#include "graphics/src/text.h"
#include "graphics/src/image.h"
#include "graphics/src/menu.h"
#include "graphics/src/textbox.h"
#include "player.h"
#include "room.h"
#include "battle.h"
#include "common.h"
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

    void setup_game();

    bool ready_to_restart() { return m_ready_to_restart; }

private:
    // std::this_thread::sleep_for except it kills the thread when m_running is set to false
    void sleep(int ms);
   
    // Lock the mutex and then emplace_back text into m_text 
    void add_text(SDL_Renderer* rend, SDL_Point pos, const std::string& text, const std::string& font_path, int ptsize, SDL_Color color, int delete_after_ms);
    // Lock the mutex and then emplace_back image into m_images
    void add_image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path, int delete_after_ms);
    // Lock the mutex and then emplace_back an std::pair<SDL_Rect, SDL_Color> into m_rects
    void add_rect(SDL_Rect rect, SDL_Color color);
    // Lock the mutex and then pop latest rect off
    void pop_rect();
    
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

    // thread this function if you are going to call it
    void game_over_sequence();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_rend;

    std::string m_resources_dir;
    std::string m_font_path;

    std::atomic<bool> m_running{ true };

    std::vector<std::unique_ptr<gui::Text>> m_text;
    std::vector<std::unique_ptr<gui::Image>> m_images;
    std::vector<std::pair<SDL_Rect, SDL_Color>> m_rects;
    std::unique_ptr<gui::Menu> m_menu;

    std::unique_ptr<gui::Textbox> m_dialogue_box;
    int m_dialogue_list_index{ 0 };

    std::unique_ptr<Battle> m_current_battle;

    std::mutex m_mtx;

    std::atomic<bool> m_z_down{ false };

    std::unique_ptr<Player> m_player;

    std::vector<std::unique_ptr<Room>> m_rooms;
    int m_current_room_index{ -1 };

    std::unique_ptr<SDL_Texture, TextureDeleter> m_atlas;
    std::map<char, SDL_Point> m_texture_map;
    std::vector<char> m_solid_characters;

    std::map<std::string, std::vector<Entity*>> m_room_entities;

    enum class Mode
    {
        CUTSCENE,
        NORMAL,
        BATTLE
    } m_mode{ Mode::CUTSCENE };

    bool m_ready_to_restart{ false };

    /* constants */
    const int m_player_speed{ 2 };
};
