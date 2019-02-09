#ifndef LR_SUPERSNAKES_GAME_HPP
#define LR_SUPERSNAKES_GAME_HPP


#include <string>
#include "engine/audio_mixer.hpp"
#include "input.hpp"
#include "engine/renderer.hpp"

class Game {
public:
    static const char *game_name;
    static const char *game_version;
    static const int game_fps = 50;
    AudioMixer mixer;

    Game(unsigned int screen_width, unsigned int screen_height, std::string core_folder_path, std::string config_folder_path);

    void GameLoop(double delta_time, std::vector<Input> controller_inputs);

    uint32_t *GetFrameBuffer();

private:
    Renderer *renderer;
};


#endif //LR_SUPERSNAKES_GAME_HPP
