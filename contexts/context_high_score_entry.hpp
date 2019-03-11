#ifndef SNAKESARENA_LIBRETRO_CONTEXT_HIGH_SCORE_ENTRY_HPP
#define SNAKESARENA_LIBRETRO_CONTEXT_HIGH_SCORE_ENTRY_HPP


#include "context.hpp"
#include "parent_context.hpp"
#include "../level.hpp"
#include "context_high_score_subwindow.hpp"

class ContextHighScoreEntry : public Context, public ParentContext {
public:
    ContextHighScoreEntry(ParentContext *game, Assets *assets, AudioMixer *mixer, SaveData *save_data,
    unsigned int screen_width, unsigned int screen_height, const Level* level, std::vector<int> player_indices,
    std::vector<int> scores);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

    void OnResume() override {};

    void PushNewContext(Context *context) override;

    void EndContext(Context *context) override;

    Renderer *GetRenderer() override;

private:
    std::vector<ContextHighScoreSubWindow*> contexts;
    std::vector<std::string> player_names;
    const Level *level;
    std::vector<int> scores;
};


#endif //SNAKESARENA_LIBRETRO_CONTEXT_HIGH_SCORE_ENTRY_HPP
