#ifndef SNAKESARENA_LIBRETRO_CONTEXT_HIGH_SCORE_SUBWINDOW_HPP
#define SNAKESARENA_LIBRETRO_CONTEXT_HIGH_SCORE_SUBWINDOW_HPP


#include "context.hpp"
#include "../level.hpp"

class ContextHighScoreSubWindow : public Context {
public:
    int x;
    int y;

    ContextHighScoreSubWindow(ParentContext *game, Assets *assets, AudioMixer *mixer, SaveData *save_data,
    unsigned int screen_width, unsigned int screen_height, const Level* level, int x, int y, int player_index, int score);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

    void OnResume() override {};

    /// Checks if the player has finished entering their name.
    /// \return True if the player is done, false otherwise.
    bool IsComplete();

    /// Gets the player's name.
    /// \return The player's name.
    std::string PlayerName();

private:
    bool running = true;
    int keyboard_index = 0;
    int player_index;
    Rect cursor_rect;
    std::string  characters;
    text_t keyboard;
    text_t player_number_text;
    text_t score_text;
    text_t name_text;
    std::string name;
    std::vector<std::pair<Texture*, Rect>> snake_border;

    void SetCharacterPositions();

    void SetBorderSprites(int winner_index);

    void SetCursor(int new_keyboard_index);

    void SetPlayerName();
};


#endif //SNAKESARENA_LIBRETRO_CONTEXT_HIGH_SCORE_SUBWINDOW_HPP
