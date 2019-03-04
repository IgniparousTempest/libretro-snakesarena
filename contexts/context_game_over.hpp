#ifndef SNAKESARENA_LIBRETRO_CONTEXT_GAME_OVER_HPP
#define SNAKESARENA_LIBRETRO_CONTEXT_GAME_OVER_HPP


#include "context.hpp"

class ContextGameOver : public Context {
public:
    ContextGameOver(GameManager *game, AudioMixer *mixer, SaveData *save_data, Assets *assets,
                    unsigned int screen_width, unsigned int screen_height, const Level* level, std::vector<int> scores, std::vector<int> wins);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

private:
    const Level* level;
    std::vector<int> scores;
    std::vector<int> wins;
    int menu_index = 0;
    uint32_t *old_frame_buffer = nullptr;
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
