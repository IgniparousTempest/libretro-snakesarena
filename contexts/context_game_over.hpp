#ifndef SNAKESARENA_LIBRETRO_CONTEXT_GAME_OVER_HPP
#define SNAKESARENA_LIBRETRO_CONTEXT_GAME_OVER_HPP


#include "context.hpp"
#include "../level.hpp"

class ContextGameOver : public Context {
public:
    ContextGameOver(ParentContext *game, AudioMixer *mixer, SaveData *save_data, Assets *assets,
                    unsigned int screen_width, unsigned int screen_height, const Level* level, std::vector<int> scores, std::vector<int> wins, const uint32_t *screen_buffer);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

    void OnResume() override {};

private:
    const Level* level;
    std::vector<int> scores;
    std::vector<int> wins;
    int menu_index = 0;
    const uint32_t *old_frame_buffer;
    Rect rect_winner;
    Rect rect_btn_play_again;
    Rect rect_btn_main_menu;
    Rect rect_cursor_left;
    Rect rect_cursor_right;
    std::vector<std::pair<Texture*, Rect>> winning_snake;

    void UpdateCursor();

    void SetWinningSnake(long long winner_index);
};


#endif //SNAKESARENA_LIBRETRO_CONTEXT_GAME_OVER_HPP
