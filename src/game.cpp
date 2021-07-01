#include "audio/src/audio.h"
#include "game.h"
#include "graphics/src/menu.h"
#include "common.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <set>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <json/json.h>
#include <json/writer.h>

namespace fs = std::filesystem;


Game::Game(const std::string& resources_path)
    : m_resources_dir(resources_path + '/')
{
    m_window = SDL_CreateWindow("Undersus", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    m_rend = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderClear(m_rend);
    SDL_RenderPresent(m_rend);

    m_font_path = m_resources_dir + "gfx/font.ttf";
    m_atlas = std::unique_ptr<SDL_Texture, TextureDeleter>(IMG_LoadTexture(m_rend, (m_resources_dir + "gfx/atlas.png").c_str()));

    m_texture_map['#'] = { 32, 0 };
    m_texture_map['.'] = { 64, 0 };
    m_solid_characters = { '#' };

    std::ifstream ifs(m_resources_dir + "maps/data.json");
    ifs >> m_json;
}


Game::Game(const std::string& resources_path, SDL_Window* window, SDL_Renderer* rend)
    : m_resources_dir(resources_path + '/'), m_window(window), m_rend(rend)
{
    m_font_path = m_resources_dir + "gfx/font.ttf";
    m_atlas = std::unique_ptr<SDL_Texture, TextureDeleter>(IMG_LoadTexture(m_rend, (m_resources_dir + "gfx/atlas.png").c_str()));

    m_texture_map['#'] = { 32, 0 };
    m_texture_map['.'] = { 64, 0 };
    m_solid_characters = { '#' };

    std::ifstream ifs(m_resources_dir + "maps/data.json");
    ifs >> m_json;
}


Game::~Game()
{
    m_atlas.reset(0); 

    // if the game is being closed instead of restarted
    if (!m_ready_to_restart)
    {
        SDL_DestroyRenderer(m_rend);
        SDL_DestroyWindow(m_window);
    }
}


void Game::mainloop()
{
    SDL_Event evt;
    
    while (m_running)
    {
        {
            std::lock_guard lock(m_mtx);

            while (SDL_PollEvent(&evt))
            {
                switch (evt.type)
                {
                case SDL_QUIT:
                    m_running = false;
                    goto cleanup;
                    break;
                case SDL_KEYDOWN:
                {
                    switch (evt.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                        if (m_menu)
                            m_menu->move_selected(1);

                        if (m_player)
                            m_player->set_x_vel(m_player_speed);

                        if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->move_selected(1);
                            m_current_battle->set_player_vx(2);
                        }

                        break;
                    case SDLK_LEFT:
                        if (m_menu)
                            m_menu->move_selected(-1);

                        if (m_player)
                            m_player->set_x_vel(-m_player_speed);

                        if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->move_selected(-1);
                            m_current_battle->set_player_vx(-2);
                        }

                        break;
                    case SDLK_UP:
                        if (m_player)
                            m_player->set_y_vel(-m_player_speed);

                        if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->set_player_vy(-2);
                        }


                        break;
                    case SDLK_DOWN:
                        if (m_player)
                            m_player->set_y_vel(m_player_speed);

                        if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->set_player_vy(2);
                        }

                        break;
                    case SDLK_z:
                        m_z_down = true;

                        if (m_mode == Mode::NORMAL)
                        {
                            Entity* ent = nearest_entity_in_range();
                            
                            if (ent)
                            {
                                if (!m_dialogue_box)
                                {
                                    m_player->set_moveable(false);
                                    m_dialogue_box = std::make_unique<gui::Textbox>(m_rend, SDL_Rect{ 20, 20, 800 - 40, 60 }, ent->dialogue()[0], m_font_path, 16, true, SDL_Color{ 0, 0, 0 }, SDL_Color{ 255, 255, 255 });
                                }
                                else
                                {
                                    ++m_dialogue_list_index;

                                    if (m_dialogue_list_index >= ent->dialogue().size())
                                    {
                                        m_player->set_moveable(true);
                                        m_dialogue_box.reset(0);
                                        m_dialogue_list_index = 0;

                                        start_battle(ent);
                                    }
                                    else
                                    {
                                        m_dialogue_box = std::make_unique<gui::Textbox>(m_rend, SDL_Rect{ 20, 20, 800 - 40, 60 }, ent->dialogue()[m_dialogue_list_index], m_font_path, 16, true, SDL_Color{ 0, 0, 0 }, SDL_Color{ 255, 255, 255 });
                                    }
                                }
                            }

                            for (auto& save : m_rooms[m_current_room_index]->saves())
                            {
                                if (within_range({ m_player->rect().x + m_player->rect().w / 2, m_player->rect().y + m_player->rect().h / 2 }, { save->rect().x + save->rect().w / 2, save->rect().y + save->rect().h / 2 }))
                                {
                                    if (!m_dialogue_box)
                                    {
                                        m_player->set_moveable(false);
                                        save_data();
                                        audio::play_sound(m_resources_dir + "sfx/savepoint.wav");
                                        m_dialogue_box = std::make_unique<gui::Textbox>(m_rend, SDL_Rect{ 20, 20, 800 - 40, 60 }, "Progress saved.", m_font_path, 16, true, SDL_Color{ 0, 0, 0 }, SDL_Color{ 255, 255, 255 });
                                    }
                                    else
                                    {
                                        m_player->set_moveable(true);
                                        m_dialogue_box.reset(0);
                                    }
                                }
                            }
                        }
                        else if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->z_down();
                            m_current_battle->hit_selected_button();
                        }

                        break;
                    }
                } break;
                case SDL_KEYUP:
                {
                    switch (evt.key.keysym.sym)
                    {
                    case SDLK_RIGHT:
                    case SDLK_LEFT:
                        if (m_player)
                            m_player->set_x_vel(0);

                        if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->set_player_vx(0);
                        }

                        break;
                    case SDLK_UP:
                    case SDLK_DOWN:
                        if (m_player)
                            m_player->set_y_vel(0); 
                        if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->set_player_vy(0);
                        }

                        break;
                    case SDLK_z:
                        m_z_down = false;

                        if (m_mode == Mode::BATTLE)
                        {
                            m_current_battle->z_up();
                        }

                        break;
                    }
                } break;
                }
            }

            audio::cleanup_sound_when_done();

            SDL_RenderClear(m_rend);

            if (m_current_room_index != -1 && m_current_room_index < m_rooms.size())
                m_rooms[m_current_room_index]->render();

            for (int i = 0; i < m_text.size(); ++i)
            {
                m_text[i]->render();

                if (m_text[i]->overtime())
                {
                    m_text.erase(m_text.begin() + i);
                    --i;
                }
            }

            for (int i = 0; i < m_images.size(); ++i)
            {
                m_images[i]->render();

                if (m_images[i]->overtime())
                {
                    m_images.erase(m_images.begin() + i);
                    --i;
                }
            }

            if (m_player)
            {
                m_player->move(m_rooms[m_current_room_index].get(), m_solid_characters);
                
                if (m_player->current_character(m_rooms[m_current_room_index].get()) == 'N')
                    next_room();

                if (m_player->current_character(m_rooms[m_current_room_index].get()) == 'P')
                    prev_room();

                m_player->render();
                m_player->animate();
            }

            if (m_menu)
                m_menu->render();

            if (m_current_battle)
            {
                m_current_battle->move_projectiles();
                m_current_battle->move_player();
                m_current_battle->check_collisions();
                m_current_battle->render();
                
                if (m_current_battle->finished())
                {
                    end_battle();
                }
            }

            for (auto& r : m_rects)
            {
                SDL_SetRenderDrawColor(m_rend, r.second.r, r.second.g, r.second.b, 255);
                SDL_RenderFillRect(m_rend, &r.first);
            }

            if (m_dialogue_box)
                m_dialogue_box->render();

            SDL_SetRenderDrawColor(m_rend, 0, 0, 0, 255);

            SDL_RenderPresent(m_rend);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

cleanup: 
    m_text.clear();
    m_images.clear();

    delete_menu();

    m_rooms.clear();

    m_player.reset(0);

    // manual reset because by the time the smart pointers go out of scope SDL_Quit() IMG_Quit() TTF_Quit() and Mix_Quit() have already been called
    
    if (m_current_battle)
        m_current_battle.reset(0);

    if (m_dialogue_box)
        m_dialogue_box.reset(0);

    if (m_menu)
        m_menu.reset(0);

    audio::stop_music();
}


void Game::start_game()
{
    bool show_image = true;

    while (true)
    {
        if (show_image)
        {
            sleep(1000);
            
            add_image(m_rend, { 0, 0 }, m_resources_dir + "gfx/logo.png", 2000);

            sleep(3000);
        }
                
        add_text(m_rend, SDL_Point{ 180, 260 }, "Press Z to select, use arrow keys to navigate.", m_font_path, 16, SDL_Color{ 255, 255, 255 }, -1);
        add_text(m_rend, SDL_Point{ 120, 300 }, "Z is the only key other than the arrow keys that you will use.", m_font_path, 16, SDL_Color{ 255, 255, 255 }, -1);

        set_menu(m_rend, { 200, 360 }, { "Start", "Reset" }, 300, m_font_path, 16);

        wait_for_z();
        
        std::string choice = get_menu_choice();

        if (choice == "Start")
        {
            {
                std::lock_guard lock(m_mtx);
                m_text.clear();
            }

            delete_menu();
            break;
        }
        else
        {
            {
                std::lock_guard lock(m_mtx);
                m_text.clear();
            }

            add_text(m_rend, SDL_Point{ 250, 340 }, "Are you sure you want to reset?", m_font_path, 16, SDL_Color{ 255, 255, 255 }, -1);
            set_menu(m_rend, SDL_Point{ 300, 420 }, { "Yes", "No" }, 140, m_font_path, 16);

            wait_for_z();

            std::string choice = get_menu_choice();

            if (choice == "Yes")
            {
                std::ifstream ifs(m_resources_dir + "maps/default_data.json");
                std::stringstream ss;
                std::string buf;

                while (std::getline(ifs, buf)) ss << buf << "\n";

                ifs.close();

                std::ofstream ofs(m_resources_dir + "maps/data.json", std::ofstream::trunc | std::ofstream::out);
                ofs << ss.str();
                ofs.close();

                ifs.open(m_resources_dir + "maps/data.json");
                ifs >> m_json;

                show_image = true;
            }
            else
            {
                show_image = false;
            }

            {
                std::lock_guard lock(m_mtx);
                m_text.clear();
            }

            delete_menu();
        }
    }

    {
        std::lock_guard lock(m_mtx);
        m_text.clear();
    }

    delete_menu();

    setup_game();
}


void Game::sleep(int ms)
{
    if (!m_running)
        exit(0); 

    auto start = std::chrono::system_clock::now();

    while (m_running && std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - start).count() <= ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}


void Game::add_text(SDL_Renderer* rend, SDL_Point pos, const std::string& text, const std::string& font_path, int ptsize, SDL_Color color, int delete_after_ms)
{
    if (!m_running)
        return;

    std::lock_guard lock(m_mtx);
    m_text.emplace_back(std::make_unique<gui::Text>(rend, pos, text, font_path, ptsize, color, delete_after_ms));
}


void Game::add_image(SDL_Renderer* rend, SDL_Point pos, const std::string& image_path, int delete_after_ms)
{
    if (!m_running)
        return;

    std::lock_guard lock(m_mtx);
    m_images.emplace_back(std::make_unique<gui::Image>(rend, pos, image_path, delete_after_ms));
}


void Game::add_rect(SDL_Rect rect, SDL_Color color)
{
    std::lock_guard lock(m_mtx);
    m_rects.emplace_back(std::pair<SDL_Rect, SDL_Color>{ rect, color });
}


void Game::pop_rect()
{
    std::lock_guard lock(m_mtx);
    m_rects.pop_back();
}


void Game::wait_for_z()
{
    while (m_running && !m_z_down)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    if (!m_running)
        exit(0);

    m_z_down = false;
}


void Game::set_menu(SDL_Renderer* rend, SDL_Point pos, const std::vector<std::string>& options, int space_between_options, const std::string& font_path, int ptsize)
{
    std::lock_guard lock(m_mtx);
    m_menu = std::make_unique<gui::Menu>(rend, pos, options, space_between_options, font_path, ptsize);
}


void Game::delete_menu()
{
    std::lock_guard lock(m_mtx);

    if (m_menu)
        m_menu.reset(0);
}


std::string Game::get_menu_choice()
{
    std::lock_guard lock(m_mtx);
    return m_menu->selected_opt();
}


void Game::open_map(const std::string& map_name)
{
    if (fs::path(map_name).extension().string() != ".txt")
        return;

    std::ifstream ifs(map_name);
    std::stringstream ss;
    std::string buf;
    
    int map_width = -1;
    
    while (std::getline(ifs, buf))
    {
        if (map_width == -1)
            map_width = buf.size();
        else
            // each row in the map should have a constant width
            if (buf.size() != map_width)
                break;

        ss << buf;
    }

    ifs.close();

    SDL_Point lpos = to_point(m_json["rooms"][fs::path(map_name).stem().string()]["start_pos"].asString());
    SDL_Point rpos = to_point(m_json["rooms"][fs::path(map_name).stem().string()]["end_pos"].asString());
    SDL_Point render_pos = to_point(m_json["rooms"][fs::path(map_name).stem().string()]["render_pos"].asString());

    lpos = { lpos.x * 32, lpos.y * 32 };
    rpos = { rpos.x * 32, rpos.y * 32 };

    {
        std::lock_guard lock(m_mtx);
        m_rooms.emplace_back(std::make_unique<Room>(m_rend, fs::path(map_name).stem().string(), ss.str(), map_width, m_texture_map, m_atlas.get(), lpos, rpos, render_pos));
        std::string room_filename = fs::path(map_name).stem().string();

        if (m_room_data.find(room_filename) != m_room_data.end())
        {
            m_rooms[m_rooms.size() - 1]->add_data(std::move(m_room_data[room_filename]));
        }

        std::vector<std::unique_ptr<Entity>> entities;

        std::vector<std::pair<std::function<void(void)>, int>> default_attacks = {
            { [&]() {
                m_current_battle->add_projectile(Projectile{ Sprite{ { 0, 0, 32, 32 }, { 100, 360, 32, 32 } }, { 2, 0 } });
                m_current_battle->add_projectile(Projectile{ Sprite{ { 0, 0, 32, 32 }, { 100, 450, 32, 32 } }, { 3, 0 } });
            }, 3000 },
            { [&]() {
                for (int i = 0; i < 10; ++i)
                {
                    m_current_battle->add_projectile(Projectile{ Sprite{ { 0, 32, 32, 32 }, { 100, 200 + i * 64, 32, 32 } }, { 4, 0 } });
                }

                for (int i = 0; i < 10; ++i)
                {
                    m_current_battle->add_projectile(Projectile{ Sprite{ { 0, 32, 32, 32 }, { 750, 232 + i * 64, 32, 32 } }, { -4, 0 } });
                }
            }, 4000 }
        };


        for (auto& e : m_json["rooms"][fs::path(map_name).stem().string()]["entities"])
        {
            std::vector<std::string> dialogue;
            std::vector<std::string> battle_dialogue;

            for (auto& str : e["dialogue"])
                dialogue.emplace_back(str.asString());

            for (auto& str : e["battle_dialogue"])
                battle_dialogue.emplace_back(str.asString());

            SDL_Point pos = to_point(e["pos"].asString());
            pos.x *= 32;
            pos.y *= 32;

            pos.x += m_rooms[m_rooms.size() - 1]->render_pos().x;
            pos.y += m_rooms[m_rooms.size() - 1]->render_pos().y;

            std::unique_ptr<Entity> entity = std::make_unique<Entity>(m_rend, pos, m_atlas.get(), to_point(e["sprite_pos"].asString()), to_point(e["dead_sprite"].asString()), to_point(e["battle_sprite"].asString()), m_resources_dir + e["theme"].asString(), dialogue, battle_dialogue, default_attacks, e["alive"].asBool());
            entities.push_back(std::move(entity));
        }

        std::vector<std::unique_ptr<Save>> saves;

        for (auto& s : m_json["rooms"][fs::path(map_name).stem().string()]["saves"])
        {
            SDL_Point pos = to_point(s["pos"].asString());
            pos.x *= 32;
            pos.y *= 32;

            pos.x += m_rooms[m_rooms.size() - 1]->render_pos().x;
            pos.y += m_rooms[m_rooms.size() - 1]->render_pos().y;

            std::unique_ptr<Save> save = std::make_unique<Save>(m_rend, m_atlas.get(), Sprite{ SDL_Rect{ 32, 32, 32, 32 }, SDL_Rect{ pos.x, pos.y, 32, 32 } });
            saves.push_back(std::move(save));
        }

        std::unique_ptr<RoomData> room_data = std::make_unique<RoomData>(entities, saves);
        m_rooms[m_rooms.size() - 1]->add_data(std::move(room_data));
    }
}


void Game::next_room()
{
    ++m_current_room_index;

    Room* room = m_rooms[m_current_room_index].get();
    m_player->move_to(room->left_start_pos().x + room->render_pos().x, room->left_start_pos().y + room->render_pos().y);
}


void Game::prev_room()
{
    --m_current_room_index;

    Room* room = m_rooms[m_current_room_index].get();
    m_player->move_to(room->right_start_pos().x + room->render_pos().x, room->right_start_pos().y + room->render_pos().y);
}



void Game::load_maps(const std::string& directory_name)
{
    std::set<std::string> files;

    for (auto& entry : fs::directory_iterator(m_resources_dir + "maps/" + directory_name))
    {
        // insert into set for predictable map ordering since set sorts alphabetically
        files.insert(entry.path().string());
    }

    for (auto& file : files)
    {
        open_map(file);
    }
}


bool Game::within_range(SDL_Point p1, SDL_Point p2)
{
    return std::abs(p2.x - p1.x) < BLOCK_SIZE && std::abs(p2.y - p1.y) < BLOCK_SIZE;
}


Entity* Game::nearest_entity_in_range()
{
    SDL_Point p1 = { m_player->rect().x + BLOCK_SIZE / 2, m_player->rect().y + BLOCK_SIZE / 2 };
    for (auto& ent : m_rooms[m_current_room_index]->entities())
    {
        SDL_Point p2 = { ent->rect().x + BLOCK_SIZE / 2, ent->rect().y + BLOCK_SIZE / 2 };

        if (within_range(p1, p2) && ent->alive())
        {
            return ent.get();
        }
    }

    return 0;
}


void Game::start_battle(Entity* ent)
{
    m_current_battle = std::make_unique<Battle>(m_rend, ent, m_atlas.get(), m_resources_dir);
    m_player->set_moveable(false);
   
    if (m_dialogue_box)
        m_dialogue_box.reset(0);

    m_mode = Mode::BATTLE;
}


void Game::end_battle()
{
    if (m_current_battle->player_dead())
    {
        audio::stop_music();
        m_current_battle.reset(0);
        m_mode = Mode::CUTSCENE;

        std::thread thr_game_over(&Game::game_over_sequence, this);
        thr_game_over.detach();
    }
    else
    {
        m_current_battle.reset(0);
        m_player->set_moveable(true);
        audio::play_music(m_resources_dir + "sfx/among_us_lofi.wav");
        m_mode = Mode::NORMAL;
    }
}


void Game::game_over_sequence()
{
    add_rect({ 0, 0, 800, 800 }, { 0, 0, 0 });
    sleep(2000);

    {
        std::lock_guard lock(m_mtx);
        m_dialogue_box = std::make_unique<gui::Textbox>(m_rend, SDL_Rect{ 300, 386, 300, 200 }, "Red was not an impostor.", m_font_path, 16, false, SDL_Color{ 0, 0, 0 }, SDL_Color{ 255, 255, 255 });
    }

    sleep(2000);

    {
        std::lock_guard lock(m_mtx);
        m_dialogue_box.reset(0);
    }

    sleep(1000);

    {
        std::lock_guard lock(m_mtx);
        m_dialogue_box = std::make_unique<gui::Textbox>(m_rend, SDL_Rect{ 300, 386, 300, 200 }, "Press z to try again...", m_font_path, 16, false, SDL_Color{ 0, 0, 0 }, SDL_Color{ 255, 255, 255 });
    }

    wait_for_z();

    if (m_running)
        m_ready_to_restart = true;

    m_running = false;
}


void Game::setup_game()
{
    load_maps("start");

    {
        std::lock_guard lock(m_mtx);

        SDL_Point pos = to_point(m_json["player"]["pos"].asString());
        m_player = std::make_unique<Player>(m_rend, SDL_Rect{ pos.x, pos.y,  BLOCK_SIZE, BLOCK_SIZE }, m_resources_dir + "gfx/sprites/player.png");
    }

    m_current_room_index = m_json["room_index"].asInt();

    audio::play_music(m_resources_dir + "sfx/among_us_lofi.wav");

    m_mode = Mode::NORMAL;
}


SDL_Point Game::to_point(const std::string& string)
{
    std::vector<std::string> split = split_string(string, ' ');
    return SDL_Point{ std::stoi(split[0]), std::stoi(split[1]) };
}


void Game::save_data()
{
    for (auto& room : m_rooms)
    {
        auto& json = m_json["rooms"][room->name()];

        json["start_pos"] = std::to_string(room->left_start_pos().x / 32) + ' ' + std::to_string(room->left_start_pos().y / 32);
        json["end_pos"] = std::to_string(room->right_start_pos().x / 32) + ' ' + std::to_string(room->right_start_pos().y / 32);
        json["render_pos"] = std::to_string(room->render_pos().x) + ' ' + std::to_string(room->render_pos().y);

        for (int i = 0; i < room->entities().size(); ++i)
        {
            auto& e = room->entities()[i];
            json["entities"][i]["alive"] = e->alive();
        }
    }

    m_json["player"]["pos"] = std::to_string(m_player->rect().x) + ' ' + std::to_string(m_player->rect().y);
    m_json["room_index"] = m_current_room_index;

    std::ofstream ofs(m_resources_dir + "maps/data.json", std::ofstream::out | std::ofstream::trunc);
    ofs << m_json << "\n";
    ofs.close();
}

