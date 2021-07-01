#pragma once
#include "entity.h"
#include "save.h"
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <SDL.h>


struct RoomData
{
    RoomData(std::vector<std::unique_ptr<Entity>>& entities, std::vector<std::unique_ptr<Save>>& saves)
        : entities(std::move(entities)), saves(std::move(saves)) {}

    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Save>> saves;
};


class Room
{
public:
    Room(SDL_Renderer* rend, const std::string& name, const std::string& layout, int characters_per_row, const std::map<char, SDL_Point>& textures, SDL_Texture* atlas, SDL_Point left_start_pos, SDL_Point right_start_pos, SDL_Point render_pos);

    void render();
    void move(int x, int y);

    bool moveable(int x, int y);

    void add_data(std::unique_ptr<RoomData> data);

    std::string layout() { return m_layout; }
    int characters_per_line() { return m_characters_per_row; }

    SDL_Point left_start_pos() { return m_left_start_pos; }
    SDL_Point right_start_pos() { return m_right_start_pos; }

    SDL_Point render_pos() { return m_render_pos; }

    std::vector<std::unique_ptr<Entity>>& entities() { return m_data->entities; }
    std::vector<std::unique_ptr<Save>>& saves() { return m_data->saves; }

    std::string name() { return m_name; }

private:
    std::string m_layout;
    int m_characters_per_row;
    std::map<char, SDL_Point> m_textures;

    SDL_Texture* m_texture_atlas;

    SDL_Renderer* m_rend;

    SDL_Point m_left_start_pos;
    SDL_Point m_right_start_pos;

    SDL_Point m_render_pos{ 0, 0 };

    std::unique_ptr<RoomData> m_data;

    std::string m_name;
};

