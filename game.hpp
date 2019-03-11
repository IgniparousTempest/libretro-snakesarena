#ifndef LR_SUPERSNAKES_GAME_HPP
#define LR_SUPERSNAKES_GAME_HPP


#include <deque>
#include "contexts/context.hpp"
#include "engine/audio_mixer.hpp"
#include "engine/renderer.hpp"
#include "assets.hpp"
#include "input.hpp"
#include "level.hpp"
#include "save_data.hpp"

class Context;

class Game : public ParentContext {
public:
    static const char *game_name;
    static const char *game_version;
    static const int game_fps = 50;
    AudioMixer mixer;

    Game(unsigned int screen_width, unsigned int screen_height, std::string core_folder_path, std::string config_folder_path);

    void GameLoop(double delta_time, std::vector<Input> controller_inputs);

    uint32_t *GetFrameBuffer();

    void PushNewContext(Context *context) override;

    void EndContext(Context *context) override;

    Renderer *GetRenderer() override;

private:
    std::vector<Level> levels;
    std::deque<Context*> contexts;
    Renderer *renderer;
    Assets *assets;
    SaveData *save_data;

    /// Reloads all levels from disk.
    /// \param level_folder_path The folder containing the level files.
    void RefreshLevels(std::string level_folder_path);
};


#endif //LR_SUPERSNAKES_GAME_HPP