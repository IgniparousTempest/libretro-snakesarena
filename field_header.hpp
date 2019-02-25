#ifndef SUPERSNAKES_LIBRETRO_FIELD_HEADER_HPP
#define SUPERSNAKES_LIBRETRO_FIELD_HEADER_HPP


#include <vector>
#include "input.hpp"
#include "engine/renderer.hpp"
#include "assets.hpp"

class FieldHeader {
public:
    FieldHeader(Assets *assets, int width, int height, unsigned int num_players);

    void Update(double delta_time, std::vector<Input> controller_inputs);

    void Render(Renderer* renderer);

    /// Changes the score for the specified player and recalculates the text to be drawn.
    /// \param player_number The player number, i.e. 1-8.
    /// \param score The score to set the player to.
    void SetScore(int player_number, int score);

private:
    Assets *assets;
    std::vector<text_t> text;
    std::vector<std::pair<Texture*, Rect>> snake_portraits;
    Rect rect;
};


#endif //SUPERSNAKES_LIBRETRO_FIELD_HEADER_HPP
