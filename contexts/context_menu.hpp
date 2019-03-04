#ifndef SUPERSNAKES_LIBRETRO_CONTEXT_MENU_HPP
#define SUPERSNAKES_LIBRETRO_CONTEXT_MENU_HPP


#include "context.hpp"
#include "../level.hpp"

class ContextMenu : public Context {
public:
    ContextMenu(GameManager *game, AudioMixer *mixer, SaveData *save_data, unsigned int screen_width,
            unsigned int screen_height, Assets *assets, std::vector<Level> *levels, unsigned int max_players);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

private:
    std::vector<Level> *levels;
    int controls_index = 0;
    int selected_level;
    unsigned int max_players;
    unsigned int num_players;
    const Texture *preview_image;
    Rect title_dest;
    Rect start_button_dest;
    Rect preview_dest;
    Rect *arrow_left_dest;
    Rect *arrow_right_dest;
    text_t players_text;
    std::vector<text_t> credits_text;

    /// Changes the displayed number of players to \ref ContextMenu.num_players.
    void UpdatePlayersText();

    /// Changes the current level preview image to the level pointed at by \ref ContextMenu.selected_level.
    void UpdatePreviewImage();

    /// Writes the game's credits.
    void GenerateCredits();

    /// Moves the selection arrows to the current control.
    void UpdateArrows();
};


#endif //SUPERSNAKES_LIBRETRO_CONTEXT_MENU_HPP
