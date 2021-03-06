#ifndef SUPERSNAKES_LIBRETRO_LEVEL_HPP
#define SUPERSNAKES_LIBRETRO_LEVEL_HPP


#include <string>
#include <vector>
#include "engine/pos.hpp"
#include "engine/texture.hpp"
#include "assets.hpp"
#include "auxiliary.hpp"

typedef std::vector<std::pair<Pos, Texture*>> walls_t;

class Level {
public:
    walls_t walls;
    std::vector<std::pair<Pos, Direction>> spawns;
    std::vector<Pos> fruit_spawn;
    std::string level_name;

    Level(std::string path, Assets *assets);

    /// Returns a preview image of the level where each tile is 4x4 px in size.
    const Texture* PreviewImage();

private:
    Texture *preview = nullptr;
    unsigned int width = 0;
    unsigned int height = 0;

    void LoadFromFile(std::string path, Assets *assets);

    void ParseToken(int x, int y, int token, Assets *assets);

    void GeneratePreview(Assets *assets);
};


#endif //SUPERSNAKES_LIBRETRO_LEVEL_HPP
