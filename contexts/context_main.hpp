#ifndef SUPERSNAKES_LIBRETRO_CONTEXT_MAIN_HPP
#define SUPERSNAKES_LIBRETRO_CONTEXT_MAIN_HPP


#include "context.hpp"
#include "../assets.hpp"
#include "../field.hpp"
#include "../field_header.hpp"
#include "../level.hpp"

class ContextMain : public Context {
public:
    ContextMain(GameManager *game, AudioMixer *mixer, SaveData *save_data, unsigned int screen_width,
            unsigned int screen_height, Assets *assets, Level* level, Renderer *renderer, unsigned int players);

    void Update(double delta_time, std::vector<Input> controller_inputs) override;

    void Render(Renderer *renderer) override;

private:
    Field *field;
    FieldHeader *field_header;
};


#endif //SUPERSNAKES_LIBRETRO_CONTEXT_MAIN_HPP
