#include <cassert>
#include <iostream>
#include <utility>
#include "level.hpp"
#include "pugixml/pugixml.hpp"
#include "engine/renderer.hpp"

Level::Level(std::string path, Assets *assets) {
    LoadFromFile(std::move(path), assets);
    GeneratePreview(assets);
}

const Texture *Level::PreviewImage() {
    return preview;
}

void Level::LoadFromFile(std::string path, Assets *assets) {
    spawns = {};
    spawns.resize(8);
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());

    if (!result)
        throw std::runtime_error("Level file not found: " + path);

    pugi::xml_node map = doc.child("map");

    width = map.attribute("width").as_uint();
    height = map.attribute("height").as_uint();
    if (width != 30 || height != 16)
        throw std::runtime_error("Invalid level file: " + path);

    auto layers = map.children("layer");
    auto data = std::string(layers.begin()->child("data").child_value());
    auto spawn_data = std::string(std::next(layers.begin())->child("data").child_value());

    std::cout << "Parsing level file (" << path << "):" << std::endl;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    int x = 0, y = 0;
    while ((pos = data.find(delimiter)) != std::string::npos) {
        token = data.substr(0, pos);
        data.erase(0, pos + delimiter.length());
        ParseToken(x, y, std::stoi(token), assets);
        if (++x >= width) {
            x = 0;
            y++;
        }
    }
    ParseToken(x, y, std::stoi(data), assets);

    std::cout << "Parsing spawn layer for level." << std::endl;
    x = 0, y = 0;
    while ((pos = spawn_data.find(delimiter)) != std::string::npos) {
        token = spawn_data.substr(0, pos);
        spawn_data.erase(0, pos + delimiter.length());
        if (std::stoi(token) != 0)
            fruit_spawn.emplace_back(x, y);
        if (++x >= width) {
            x = 0;
            y++;
        }
    }
    if (std::stoi(data) != 0)
        fruit_spawn.emplace_back(x, y);

    // Read properties
    const char *name, *value;
    for (pugi::xml_node property : map.child("properties").children("property")) {
        name = property.attribute("name").value();
        value = property.attribute("value").value();
        std::cout << name << ": " << value << std::endl;
        if (std::strcmp(name, "name") == 0) {
            level_name = std::string(value);
        }
    }
}

void Level::ParseToken(int x, int y, int token, Assets *assets) {
    // Is a player spawn
    if (token >= 9 && token <= 50) {
        int player_number = (token - 9) % 8;
        int direction_index = (token - 9) / 8;
        Direction direction = RIGHT;
        if (direction_index == 0)
            direction = RIGHT;
        else if (direction_index == 1)
            direction = DOWN;
        else if (direction_index == 2)
            direction = UP;
        else if (direction_index == 3)
            direction = LEFT;
        spawns[player_number] = {{x, y}, direction};
    } else
        switch (token) {
            // Walls
            case 1:
                walls.push_back({{x, y}, assets->wall_top});
                break;
            case 3:
                walls.push_back({{x, y}, assets->wall_bottom});
                break;
            case 4:
                walls.push_back({{x, y}, assets->wall_left});
                break;
            case 2:
                walls.push_back({{x, y}, assets->wall_right});
                break;
            case 8:
                walls.push_back({{x, y}, assets->wall_top_left});
                break;
            case 5:
                walls.push_back({{x, y}, assets->wall_top_right});
                break;
            case 6:
                walls.push_back({{x, y}, assets->wall_bottom_right});
                break;
            case 7:
                walls.push_back({{x, y}, assets->wall_bottom_left});
                break;

            // Unknown token
            default:
                break;
        }
}

void Level::GeneratePreview(Assets *assets) {
    Renderer renderer(width * 4, height * 4);
    Rect dest = {0, 0, 4, 4};

    // Background
    auto colour_light = Renderer::rgb(170, 215, 81);
    auto colour_dark = Renderer::rgb(162, 209, 73);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            dest.x = x * 4;
            dest.y = y * 4;
            renderer.RenderSquare((x + y) % 2 == 0 ? colour_light : colour_dark, &dest);
        }
    }

    // Walls
    auto colour_wall = Renderer::rgb(255, 97, 29);
    for (auto &wall : walls) {
        dest.x = wall.first.x * 4;
        dest.y = wall.first.y * 4;
        renderer.RenderSquare(colour_wall, &dest);
    }

    // Player spawns
    auto snake_iter = assets->snakes.begin();
    Rect inner = {0, 0, 2, 2};
    for (auto &spawn : spawns) {
        dest.x = spawn.first.x * 4;
        dest.y = spawn.first.y * 4;
        renderer.RenderSquare((*snake_iter)->body_straight->At(0, 5), &dest);
        inner.x = dest.x + 1;
        inner.y = dest.y + 1;
        renderer.RenderSquare((*snake_iter)->body_straight->At(0, 7), &inner);
        snake_iter++;
    }

    // Return copy
    auto image_size = renderer.width * renderer.height;
    auto image = new uint32_t[image_size];
    std::copy(renderer.framebuffer, renderer.framebuffer + image_size, image);
    preview = new Texture(image, renderer.width, renderer.height);
}
