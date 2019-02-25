#ifndef SUPERSNAKES_LIBRETRO_BACKGROUND_HPP
#define SUPERSNAKES_LIBRETRO_BACKGROUND_HPP


#include <deque>
#include <random>
#include "engine/audio_mixer.hpp"
#include "engine/renderer.hpp"
#include "assets.hpp"
#include "field_header.hpp"
#include "input.hpp"
#include "level.hpp"
#include "snake.hpp"

class Snake;

class Field {
public:
    Field(int square_size, int field_width, int field_height, int y_offset, Assets* assets, AudioMixer *mixer, Level* level, int num_players);

    void Update(double delta_time, std::vector<Input> controller_inputs, FieldHeader* header);

    void PartialRender(Renderer* renderer);

    void RenderAll(Renderer* renderer);

    bool CollectFood(Pos pos);

    bool CollideWithSnake(Snake *player, const Pos pos);

    bool CollideWithOtherSnakes(Snake *player, Pos pos);

    bool CollideWithWall(Pos pos);

    int FieldWidth() {
        return field_width;
    }

    int FieldHeight() {
        return field_height;
    }

    int ScreenX(int x) {
        return square_size * x;
    }

    int ScreenY(int y) {
        return square_size * y + y_offset;
    }

    void RenderSquare(Renderer* renderer, Pos pos) {
        Rect dest = {ScreenX(pos.x), ScreenY(pos.y), square_size, square_size};
        renderer->RenderSquare((pos.x + pos.y) % 2 == 0 ? colour_light : colour_dark, &dest);
    }

private:
    std::mt19937 rng;
    int new_food = 0;
    int square_size;
    int field_width;
    int field_height;
    int y_offset;
    uint32_t colour_light;
    uint32_t colour_dark;
    std::deque<Pos> food;
    AudioMixer *mixer;
    Texture *raspberry;
    Sound *snd_chomp;
    Sound *snd_crash;
    std::vector<Pos> redraws_required;
    std::vector<Snake> players;
    Level* level;
};


#endif //SUPERSNAKES_LIBRETRO_BACKGROUND_HPP
