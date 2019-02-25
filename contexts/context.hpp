#include <utility>

#ifndef SUPERSNAKES_LIBRETRO_CONTEXT_HPP
#define SUPERSNAKES_LIBRETRO_CONTEXT_HPP

#include <vector>
#include "../engine/audio_mixer.hpp"
#include "../engine/renderer.hpp"
#include "../assets.hpp"
#include "../game_manager.hpp"
#include "../input.hpp"
#include "../save_data.hpp"

class GameManager;

class Context {
public:
    explicit Context(GameManager *game, Assets *assets, AudioMixer *mixer, SaveData *save_data,
            unsigned int screen_width, unsigned int screen_height, std::string context_name) :
            game_manager(game), assets(assets), mixer(mixer), screen_width(screen_width), screen_height(screen_height),
            save_data(save_data), context_name(std::move(context_name)) {}

    ~Context() = default;

    virtual void Update(double delta_time, std::vector<Input> controller_inputs) = 0;

    virtual void Render(Renderer *renderer) = 0;

    std::string Name() {
        return context_name;
    }

protected:
    GameManager *game_manager;
    Assets *assets;
    AudioMixer *mixer;
    SaveData *save_data;
    unsigned int screen_width;
    unsigned int screen_height;
    std::string context_name;
};


#endif //SUPERSNAKES_LIBRETRO_CONTEXT_HPP
