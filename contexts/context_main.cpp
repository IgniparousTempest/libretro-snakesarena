#include "context_main.hpp"

ContextMain::ContextMain(GameManager *game, AudioMixer *mixer, SaveData *save_data, unsigned int screen_width,
        unsigned int screen_height, Assets *assets, Level* level, Renderer *renderer, unsigned int players) :
        Context(game, assets, mixer, save_data, screen_width, screen_height, "Main Game Context") {
    int square_size = 16;
    int header_height = 14;
    int field_width = screen_width / square_size;
    int field_height = (screen_height - header_height) / square_size;
    field = new Field(square_size, field_width, field_height, header_height, assets, mixer, level, players);
    field->RenderAll(renderer);
    field_header = new FieldHeader(assets, screen_width, header_height, players);

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, static_cast<unsigned int>(assets->snd_background.size() - 1));
    mixer->Play(assets->snd_background[dist(rng)]);
}

void ContextMain::Update(double delta_time, std::vector<Input> controller_inputs) {
    // Update Field
    field->Update(delta_time, controller_inputs, field_header);
    field_header->Update(delta_time, controller_inputs);
}

void ContextMain::Render(Renderer *renderer) {
    field->PartialRender(renderer);
    field_header->Render(renderer);
}
